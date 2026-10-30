#pragma once

#include <cstdint>
namespace slay {
struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 0;
    static constexpr Color rgba(uint32_t hex) {
        return {uint8_t(hex >> 24), uint8_t(hex >> 16), uint8_t(hex >> 8), uint8_t(hex)};
    }
};

struct Rect {
    float x = 0, y = 0, w = 0, h = 0;
};

struct CornerRadius {
    float topLeft = 0, topRight = 0, bottomLeft = 0, bottomRight = 0;
    static constexpr CornerRadius all(float v) { return {v, v, v, v}; }
};

struct Paddings {
    uint16_t left = 0, right = 0, top = 0, bottom = 0;

    static constexpr Paddings all(uint16_t v) { return {v, v, v, v}; }
    static constexpr Paddings x(uint16_t v) { return {v, v, 0, 0}; }
    static constexpr Paddings x(uint16_t l, uint16_t r) { return {l, r, 0, 0}; }
    static constexpr Paddings y(uint16_t v) { return {0, 0, v, v}; }
    static constexpr Paddings y(uint16_t t, uint16_t b) { return {0, 0, t, b}; }
};

enum class SizeKind : uint8_t { Fit, Grow, Fixed, Percent };

struct Extent {
    SizeKind kind = SizeKind::Fit;
    float value = 0;
    float min = 0, max = 0;

    constexpr Extent minPx(float v) const {
        Extent e = *this;
        e.min = v;
        return e;
    }
    constexpr Extent maxPx(float v) const {
        Extent e = *this;
        e.max = v;
        return e;
    }
};

constexpr Extent px(float v) {
    return {SizeKind::Fixed, v, 0, 0};
}
// Uses 0-1 scale
constexpr Extent pct(float v) {
    return {SizeKind::Percent, v, 0, 0};
}
constexpr Extent grow() {
    return {SizeKind::Grow, 0, 0, 0};
}
constexpr Extent fit() {
    return {SizeKind::Fit, 0, 0, 0};
}

// FontId value 0 - is error/absence of font.
struct FontId {
    uint16_t value = 0;
    explicit operator bool() const { return value != 0; }
};

enum class Direction : uint8_t { Row, Column };

enum class Align : uint8_t { Start, Center, End };

struct Border {
    Color color;
    uint16_t width = 0;
};

} // namespace slay
