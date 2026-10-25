#define CLAY_IMPLEMENTATION
#include "slay/layout_engine.h"

#include "slay/font_manager.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <clay.h>

static Clay_Dimensions measureText(Clay_StringSlice text, Clay_TextElementConfig* config,
                                   void* userData) {
    slay::FontManager* manager = static_cast<slay::FontManager*>(userData);
    int width, height;

    TTF_Font* font = manager->resolve(slay::FontId{config->fontId}, config->fontSize);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Failed to measure text: font id %d at size %d not resolved", config->fontId,
                     config->fontSize);
        return {0.0f, 0.0f};
    }
    if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s", SDL_GetError());
    }
    return {static_cast<float>(width), static_cast<float>(height)};
}

static void clayError(Clay_ErrorData error) {
    SDL_Log("Clay error: %.*s", error.errorText.length, error.errorText.chars);
}

bool slay::LayoutEngine::init(float width, float height) {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    m_clayArena.resize(totalMemorySize);
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, m_clayArena.data());

    if (!Clay_Initialize(arena, {width, height}, {clayError, nullptr}))
        return false;
    return true;
}

void slay::LayoutEngine::setFontManager(slay::FontManager* manager) {
    m_fontManager = manager;
    Clay_SetMeasureTextFunction(measureText, manager);
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
