#include "slay/core/arena.h"
#include "slay/ui/div.h"
#include "slay/ui/text.h"

#include <clay.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

static bool clayError = false;

static void onClayError(Clay_ErrorData) {
    clayError = true;
}

static Clay_Dimensions fakeMeasure(Clay_StringSlice s, Clay_TextElementConfig*, void*) {
    return {s.length * 10.0f, 20.0f};
}

static Clay_RenderCommandArray layout(slay::Element& root) {
    clayError = false;
    Clay_BeginLayout();
    root.emit();
    return Clay_EndLayout(0);
}

static bool boxIs(Clay_RenderCommandArray& cmds, int32_t i, float x, float y, float w, float h) {
    Clay_BoundingBox b = Clay_RenderCommandArray_Get(&cmds, i)->boundingBox;
    return b.x == x && b.y == y && b.width == w && b.height == h;
}

int main() {
    uint32_t size = Clay_MinMemorySize();
    std::vector<char> memory(size);
    Clay_Initialize(Clay_CreateArenaWithCapacityAndMemory(size, memory.data()), {800, 600},
                    {onClayError, nullptr});
    Clay_SetMeasureTextFunction(fakeMeasure, nullptr);

    const slay::Color bg = slay::Color::rgba(0x808080ff);

    {
        slay::Arena arena(4096);
        auto text = slay::TextElement(arena, "hello").font(slay::FontId{1});
        auto bar = slay::DivElement(arena).width(slay::grow()).height(slay::px(50)).bg(bg);
        auto root = slay::DivElement(arena)
                        .width(slay::px(400))
                        .height(slay::px(300))
                        .pad(slay::Paddings::all(10))
                        .dir(slay::Direction::Column)
                        .bg(bg)
                        .child(&bar)
                        .child(&text);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 3);
        assert(boxIs(cmds, 0, 0, 0, 400, 300));
        assert(boxIs(cmds, 1, 10, 10, 380, 50));
        assert(boxIs(cmds, 2, 10, 60, 50, 20));
        assert(Clay_RenderCommandArray_Get(&cmds, 2)->commandType ==
               CLAY_RENDER_COMMAND_TYPE_TEXT);
        std::cout << "Column with padding and text: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto fixed = slay::DivElement(arena).width(slay::px(100)).height(slay::px(20)).bg(bg);
        auto rest = slay::DivElement(arena).width(slay::grow()).height(slay::px(20)).bg(bg);
        auto root = slay::DivElement(arena)
                        .width(slay::px(300))
                        .height(slay::px(100))
                        .child(&fixed)
                        .child(&rest);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 2);
        assert(boxIs(cmds, 0, 0, 0, 100, 20));
        assert(boxIs(cmds, 1, 100, 0, 200, 20));
        std::cout << "Grow fills remaining space: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto a = slay::DivElement(arena).width(slay::px(50)).height(slay::px(20)).bg(bg);
        auto b = slay::DivElement(arena).width(slay::px(50)).height(slay::px(20)).bg(bg);
        auto root =
            slay::DivElement(arena).dir(slay::Direction::Column).gap(10).child(&a).child(&b);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 2);
        assert(boxIs(cmds, 0, 0, 0, 50, 20));
        assert(boxIs(cmds, 1, 0, 30, 50, 20));
        std::cout << "Gap: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto box = slay::DivElement(arena).width(slay::px(50)).height(slay::px(50)).bg(bg);
        auto root = slay::DivElement(arena)
                        .width(slay::px(200))
                        .height(slay::px(100))
                        .alignX(slay::Align::Center)
                        .alignY(slay::Align::Center)
                        .child(&box);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 1);
        assert(boxIs(cmds, 0, 75, 25, 50, 50));
        std::cout << "Align center: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto half = slay::DivElement(arena).width(slay::pct(0.5f)).height(slay::px(10)).bg(bg);
        auto root =
            slay::DivElement(arena).width(slay::px(400)).height(slay::px(10)).child(&half);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 1);
        assert(boxIs(cmds, 0, 0, 0, 200, 10));
        std::cout << "Percent width: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto child =
            slay::DivElement(arena).width(slay::grow().minPx(100)).height(slay::px(10)).bg(bg);
        auto root =
            slay::DivElement(arena).width(slay::px(50)).height(slay::px(10)).child(&child);

        Clay_RenderCommandArray cmds = layout(root);
        assert(!clayError);
        assert(cmds.length == 1);
        assert(boxIs(cmds, 0, 0, 0, 100, 10));
        std::cout << "Grow respects min: PASS\n";
    }

    {
        slay::Arena arena(4096);
        auto bad = slay::DivElement(arena).width(slay::pct(50)).height(slay::px(10));
        auto root = slay::DivElement(arena).width(slay::px(100)).child(&bad);

        layout(root);
        assert(clayError);
        std::cout << "Percent over 1 is an error: PASS\n";
    }

    std::cout << "All tests passed.\n";
}
