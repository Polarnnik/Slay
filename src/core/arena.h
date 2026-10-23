#pragma once

#include <cstddef>
#include <cstring>
#include <cstdint>
#include <memory>
#include <new>
#include <stdexcept>
#include<type_traits>
#include <utility>

class Arena {
  public:
    Arena(size_t size);
    void reset();
    template<class T, class... Args>
    T* alloc(Args&&... args) {
        static_assert(std::is_trivially_destructible_v<T>);
        void* mem = allocRaw(sizeof(T), alignof(T));
        if (!mem) throw std::bad_alloc();
        return new(mem) T {std::forward<Args>(args)...};
    }
    void* allocRaw(size_t size, size_t align) noexcept;
    size_t used() const;
    size_t capacity() const;

  private:

    std::unique_ptr<std::byte[]> m_memBlock;
    std::byte* m_pointer = nullptr;
    std::byte* m_end = nullptr;
};

template<class T>
class ArenaArray {
  public:
    ArenaArray() = default;
    void push(Arena& arena, const T& val);
    const T* begin() const { return m_data; };
    const T* end() const { return m_data ? m_data + m_len : nullptr;; };
    uint32_t len() const { return m_len; }
    uint32_t capacity() const { return m_cap; }
  private:
    void reallocate(Arena& arena, uint32_t newCap);

    T* m_data = nullptr;
    uint32_t m_len = 0;
    uint32_t m_cap = 0;
};

template<class T>
void ArenaArray<T>::push(Arena& arena, const T& val) {
    if (m_cap == m_len) {
        const uint32_t newCap = (m_cap == 0) ? 4 : m_cap * 2;
        reallocate(arena, newCap);
    }

    m_data[m_len] = val;
    ++m_len;
}

template<class T>
void ArenaArray<T>::reallocate(Arena& arena, uint32_t newCap) {
    static_assert(std::is_trivially_copyable_v<T>);
    void* newArea = arena.allocRaw(newCap * sizeof(T), alignof(T));
    T* temp = static_cast<T*>(newArea);

    if (temp == nullptr) throw  std::runtime_error("Couldn't reallocate more space for arena array.");
    if (m_len > 0) std::memcpy(temp, m_data, m_len * sizeof(T));
    m_data = temp;
    m_cap = newCap;
}
