#pragma once
#include "i_renderer.h"
#include "slay/font_manager.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_video.h>

namespace slay {

class SDLRenderer : public IRenderer {
  public:
    explicit SDLRenderer(SDL_Window* window, FontManager& manager) : m_window(window), m_fontManager(manager) {}
    ~SDLRenderer();

    bool init() override;

    void beginFrame() override;
    void render(RenderCommandArray& cmd) override;
    void endFrame() override;

  private:
    void drawRectangle(const Clay_RenderCommand& cmd);
    void drawText(const Clay_RenderCommand& cmd);
    void drawBorder(const Clay_RenderCommand& cmd);
    void drawImage(const Clay_RenderCommand& cmd);
    void startScissors(const Clay_RenderCommand& cmd);
    void endScissors(const Clay_RenderCommand& cmd);

    void drawRoundedRect(const Clay_RenderCommand& cmd, const SDL_FRect& rect);
    void drawArc(const Clay_RenderCommand& cmd, const SDL_FPoint center, const float radius, const float startAngle, const float endAngle, const float thickness, const Clay_Color color);

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_TextEngine* m_textEngine;
    FontManager& m_fontManager;
};
}
