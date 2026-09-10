#pragma once

#include "slay/font_manager.h"
#include <clay.h>
#include <cstddef>
#include <vector>

namespace slay {

using RenderCommandArray = Clay_RenderCommandArray;

class LayoutEngine {
  public:
    bool init(float width, float height);
    void begin();
    RenderCommandArray end(float deltaTime);
    void setWindowSize(float width, float height);
    void setFontManager(slay::FontManager* fontManager);

  private:
    std::vector<std::byte> m_clayArena;
    FontManager* m_fontManager;
};
}
