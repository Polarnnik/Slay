#include "../src/core/arena.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <new>

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
Arena arena(1024);

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
    Arena arena(1024);

    AlignedData* data = arena.alloc<AlignedData>();
    assert(reinterpret_cast<std::uintptr_t>(data)
           % alignof(AlignedData) == 0);

    data->value = 123;
    assert(data->value == 123);

    std::cout << "Alignment: PASS\n";
}

// Test 3: ArenaArray growth and values
{
    Arena arena(4096);
    ArenaArray<int> arr;

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
    Arena arena(16);

    bool threw = false;

    assert(arena.allocRaw(1024, alignof(int)) == nullptr);

    try { arena.alloc<std::array<int, 64>>(); }
    catch (const std::bad_alloc&) { threw = true; }
    assert(threw);
    std::cout << "Arena exhaustion: PASS\n";
}

// Test 5: Reset
{
    Arena arena(1024);

    arena.alloc<int>(42);
    arena.reset();

    assert(arena.used() == 0);

    int* number = arena.alloc<int>(99);
    assert(*number == 99);

    std::cout << "Reset: PASS\n";
}

std::cout << "All tests passed.\n";

}
