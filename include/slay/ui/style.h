#pragma once
#include <cstdint>

#include "types.h"

namespace slay {

struct Style {
    Extent width = fit();
    Extent height = fit();
    Paddings paddings{};
    uint16_t gap = 0;
    Direction direction = Direction::Row;
    Align alignX = Align::Start, alignY = Align::Start;
    Color background{};
    CornerRadius corner{};
    Border border{};
};

struct TextConfig {
    FontId font;
    uint16_t size = 16;
    Color color = Color::rgba(0xFFFFFFFF);
};

} // namespace slay
