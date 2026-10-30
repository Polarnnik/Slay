#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <new>

#include "../include/slay/core/arena.h"

struct Point {
    int x;
    int y;
};

struct alignas(32) AlignedData {
    int value;
};

int main() {
    // Test 1: Basic allocation
    {
        slay::Arena arena(1024);

        int* number = arena.alloc<int>(42);
        assert(number != nullptr);
        assert(*number == 42);

        Point* point = arena.alloc<Point>(10, 20);
        assert(point->x == 10);
        assert(point->y == 20);

        std::cout << "Basic allocation: PASS\n";
    }

    // Test 2: Alignment
    {
        slay::Arena arena(1024);

        AlignedData* data = arena.alloc<AlignedData>();
        assert(reinterpret_cast<std::uintptr_t>(data) % alignof(AlignedData) == 0);

        data->value = 123;
        assert(data->value == 123);

        std::cout << "Alignment: PASS\n";
    }

    // Test 3: ArenaArray growth and values
    {
        slay::Arena arena(4096);
        slay::ArenaArray<int> arr;

        for (int i = 0; i < 10; ++i) {
            arr.push(arena, i * 10);
        }

        assert(arr.len() == 10);
        assert(arr.capacity() >= arr.len());

        for (std::uint32_t i = 0; i < arr.len(); ++i) {
            assert(arr.begin()[i] == static_cast<int>(i * 10));
        }

        std::cout << "ArenaArray growth: PASS\n";
    }

    // Test 4: Arena exhaustion
    {
        slay::Arena arena(16);

        bool threw = false;

        assert(arena.allocRaw(1024, alignof(int)) == nullptr);

        try {
            arena.alloc<std::array<int, 64>>();
        } catch (const std::bad_alloc&) {
            threw = true;
        }
        assert(threw);
        std::cout << "Arena exhaustion: PASS\n";
    }

    // Test 5: Reset
    {
        slay::Arena arena(1024);

        arena.alloc<int>(42);
        arena.reset();

        assert(arena.used() == 0);

        int* number = arena.alloc<int>(99);
        assert(*number == 99);

        std::cout << "Reset: PASS\n";
    }

    // Test 6: Independence
    {
        slay::Arena arena(64);
        std::string src = "hello";
        slay::ArenaStr s = arena.copyStr(src);

        src[0] = 'J';
        src.clear();
        src.shrink_to_fit();

        assert(s.len == 5);
        assert(std::memcmp(s.data, "hello", 5) == 0);
        std::cout << "String independence: PASS\n";
    }

    // Test 7: exhaustion
    {
        slay::Arena arena(4);
        const size_t before = arena.used();
        bool threw = false;
        try {
            arena.copyStr("too long");
        } catch (const std::bad_alloc&) {
            threw = true;
        }
        assert(threw);
        assert(arena.used() == before);
        std::cout << "String exhaustion: PASS\n";
    }

    std::cout << "All tests passed.\n";
}
