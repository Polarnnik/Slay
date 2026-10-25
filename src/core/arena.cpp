#include <cassert>
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>
#include "slay/core/arena.h"

slay::Arena::Arena(size_t size) {
    m_memBlock = std::make_unique_for_overwrite<std::byte[]>(size);
    m_pointer = m_memBlock.get();
    m_end = m_memBlock.get() + size;
}

void slay::Arena::reset() {
    m_pointer = m_memBlock.get();
}

void* slay::Arena::allocRaw(size_t size, size_t align) noexcept {
    assert(align != 0 && (align & (align - 1)) == 0);

    const auto cur = reinterpret_cast<uintptr_t>(m_pointer);
    const auto end = reinterpret_cast<uintptr_t>(m_end);
    const uintptr_t aligned = (cur + align - 1) & ~(uintptr_t(align) - 1);
    if (aligned > end || size > end - aligned) return nullptr;

    m_pointer = reinterpret_cast<std::byte*>(aligned + size);
    return reinterpret_cast<void*>(aligned);
}

size_t slay::Arena::used() const {
    return static_cast<size_t>(m_pointer - m_memBlock.get());
}

size_t slay::Arena::capacity() const {
    return static_cast<size_t>(m_end - m_memBlock.get());
}

slay::ArenaStr slay::Arena::copyStr(std::string_view str) {
    void* arenaStr = allocRaw(str.size(), 1);
    if (arenaStr == nullptr) throw std::bad_alloc();

    std::memcpy(arenaStr, str.data(), str.size());
    return { static_cast<char*>(arenaStr), static_cast<uint32_t>(str.size()) };
}
