#pragma once
#include "slay/layout_engine.h"
#include <clay.h>

namespace slay {
class IRenderer {
    public:
    virtual ~IRenderer() = default;
    virtual bool init() = 0;
    virtual void beginFrame() = 0;
    virtual void render(RenderCommandArray& cmd) = 0;
    virtual void endFrame() = 0;
    virtual void shutdown() = 0;
};
}
