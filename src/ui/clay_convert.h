#pragma once
#include "slay/ui/style.h"
#include "slay/ui/types.h"

#include <clay.h>

inline Clay_Color toClay(slay::Color c) {
    return {
        c.r / 255.0f,
        c.g / 255.0f,
        c.b / 255.0f,
        c.a / 255.0f,
    };
}

inline Clay_CornerRadius toClay(slay::CornerRadius c) {
    return {c.topLeft, c.topRight, c.bottomLeft, c.bottomRight};
}

inline Clay_Padding toClay(slay::Paddings p) {
    return {p.left, p.right, p.top, p.bottom};
}

inline Clay_SizingAxis toClay(slay::Extent e) {
    switch (e.kind) {
    case slay::SizeKind::Fixed:
        return CLAY_SIZING_FIXED(e.value);
    case slay::SizeKind::Grow:
        return CLAY_SIZING_GROW(e.min, e.max);
    case slay::SizeKind::Fit:
        return CLAY_SIZING_FIT(e.min, e.max);
    case slay::SizeKind::Percent:
        return CLAY_SIZING_PERCENT(e.value);
    }
    return CLAY_SIZING_FIT(0, 0);
}

inline Clay_LayoutDirection toClay(slay::Direction d) {
    switch (d) {
    case slay::Direction::Row:
        return Clay_LayoutDirection::CLAY_LEFT_TO_RIGHT;
    case slay::Direction::Column:
        return Clay_LayoutDirection::CLAY_TOP_TO_BOTTOM;
    }

    return Clay_LayoutDirection::CLAY_LEFT_TO_RIGHT;
}

inline Clay_LayoutAlignmentX toClay(slay::Align a) {
    switch (a) {
    case slay::Align::Start:
        return CLAY_ALIGN_X_LEFT;
    case slay::Align::Center:
        return CLAY_ALIGN_X_CENTER;
    case slay::Align::End:
        return CLAY_ALIGN_X_RIGHT;
    }

    return CLAY_ALIGN_X_LEFT;
}

inline Clay_LayoutAlignmentY toClayY(slay::Align a) {
    switch (a) {
    case slay::Align::Start:
        return CLAY_ALIGN_Y_TOP;
    case slay::Align::Center:
        return CLAY_ALIGN_Y_CENTER;
    case slay::Align::End:
        return CLAY_ALIGN_Y_BOTTOM;
    }
    return CLAY_ALIGN_Y_TOP;
}

inline Clay_ElementDeclaration toClay(const slay::Style& s) {
    Clay_ElementDeclaration d = {};

    d.layout = {
        .sizing = {toClay(s.width), toClay(s.height)},
        .padding = toClay(s.paddings),
        .childGap = s.gap,
        .childAlignment = {toClay(s.alignX), toClayY(s.alignY)},
        .layoutDirection = toClay(s.direction),
    };
    d.backgroundColor = toClay(s.background);
    d.cornerRadius = toClay(s.corner);
    if (s.border.width > 0)
        d.border = {.color = toClay(s.border.color),
                    .width = {s.border.width, s.border.width, s.border.width, s.border.width}};
    return d;
}

inline Clay_TextElementConfig toClay(const slay::TextConfig& t) {
    Clay_TextElementConfig c{};
    c.textColor = toClay(t.color);
    c.fontId = t.font.value;
    c.fontSize = t.size;
    return c;
}
