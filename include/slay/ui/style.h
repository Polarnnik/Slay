#pragma once
#include "types.h"

namespace slay {

    struct Style {
        Extent width = fit();
        Extent height = fit();
        Paddings paddings {};
        float gap = 0;
        Direction direction = Direction::Row;
        Align alignX = Align::Start, alignY = Align::Start;
        Color background {};
        CornerRadius corner {};
        Border border {};
    };

    struct TextConfig {
        FontId font;
        float size = 16;
        Color color = Color::rgba(0xFFFFFFFF);
    };

}
