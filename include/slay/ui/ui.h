#pragma once
#include "slay/core/arena.h"
#include "slay/ui/div.h"
#include "slay/ui/text.h"
#include "slay/ui/types.h"

namespace slay {

class Ui {
  public:
    explicit Ui(size_t arenaBytes = 1 << 20) : m_arena(arenaBytes) {}
    void beginFrame() { m_arena.reset(); }
    void setDefaultFont(FontId f) { m_defaultFont = f; }

    DivElement div() { return DivElement(m_arena); }
    TextElement text(std::string_view s) { return TextElement(m_arena, s).font(m_defaultFont); }

  private:
    Arena m_arena;
    FontId m_defaultFont;
};
} // namespace slay
