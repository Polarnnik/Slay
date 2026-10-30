#pragma once
#include "slay/core/arena.h"
#include "slay/ui/element.h"
#include "slay/ui/style.h"
#include "slay/ui/types.h"

#include <string_view>
#include <utility>

namespace slay {

class TextElement final : public Element {
  public:
    explicit TextElement(Arena& arena, std::string_view str) : m_str(arena.copyStr(str)) {}

    TextElement&& font(FontId f) && {
        m_textConf.font = f;
        return std::move(*this);
    }

    TextElement&& sz(float s) && {
        m_textConf.size = s;
        return std::move(*this);
    }

    TextElement&& clr(Color c) && {
        m_textConf.color = c;
        return std::move(*this);
    }

    void emit() override;

  private:
    ArenaStr m_str;
    TextConfig m_textConf{};
};
} // namespace slay
