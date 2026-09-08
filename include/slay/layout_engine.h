#pragma once

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

  private:
    std::vector<std::byte> m_clayArena;
};
}
