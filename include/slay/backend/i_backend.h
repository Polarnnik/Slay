#pragma once
#include <clay.h>

namespace slay {
class IBackend {
    public:
    virtual ~IBackend() = default;
    virtual void beginFrame() = 0;
    virtual void render(float deltaTime) = 0;
    virtual void endFrame() = 0;
};
}
