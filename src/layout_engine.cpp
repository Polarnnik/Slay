#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <SDL3/SDL.h>
#include "slay/layout_engine.h"


static void clayError(Clay_ErrorData error)
{
    SDL_Log("Clay error: %s", error.errorText);
}

bool slay::LayoutEngine::init(float width, float height) {
     uint64_t totalMemorySize = Clay_MinMemorySize();
     m_clayArena.resize(totalMemorySize);
     Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, m_clayArena.data());

     if (!Clay_Initialize(arena, {width, height}, {clayError, nullptr}))
         return false;
     return true;
}

void slay::LayoutEngine::begin() {
    Clay_BeginLayout();
}

slay::RenderCommandArray slay::LayoutEngine::end(float deltaTime) {
    return Clay_EndLayout(deltaTime);
}

void slay::LayoutEngine::setWindowSize(float width, float height) {
    Clay_SetLayoutDimensions({width, height});
}
