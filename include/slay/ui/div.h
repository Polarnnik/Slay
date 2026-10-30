#pragma once

#include "slay/core/arena.h"
#include "slay/ui/element.h"
#include "slay/ui/style.h"
#include "slay/ui/types.h"

#include <utility>

namespace slay {
class DivElement final : public Element {
  public:
    explicit DivElement(Arena& arena) : m_arena(&arena) {}

    DivElement&& width(Extent v) && {
        m_style.width = v;
        return std::move(*this);
    }
    DivElement&& height(Extent v) && {
        m_style.height = v;
        return std::move(*this);
    }
    DivElement&& pad(Paddings p) && {
        m_style.paddings = p;
        return std::move(*this);
    }
    DivElement&& gap(float f) && {
        m_style.gap = f;
        return std::move(*this);
    }
    DivElement&& dir(Direction d) && {
        m_style.direction = d;
        return std::move(*this);
    }
    DivElement&& alignX(Align a) && {
        m_style.alignX = a;
        return std::move(*this);
    }
    DivElement&& alignY(Align a) && {
        m_style.alignY = a;
        return std::move(*this);
    }
    DivElement&& rad(CornerRadius r) && {
        m_style.corner = r;
        return std::move(*this);
    }
    DivElement&& bg(Color c) && {
        m_style.background = c;
        return std::move(*this);
    }
    DivElement&& bd(Border b) && {
        m_style.border = b;
        return std::move(*this);
    }

    DivElement&& child(Element* e) && {
        if (e)
            m_children.push(*m_arena, e);
        return std::move(*this);
    }

    void emit() override;

  private:
    Arena* m_arena = nullptr;
    Style m_style;
    ArenaArray<Element*> m_children;
};
} // namespace slay
