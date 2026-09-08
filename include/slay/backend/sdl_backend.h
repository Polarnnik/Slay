#pragma once
#include "i_backend.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace slay {
class SDLBackend : public IBackend {
  public:
    explicit SDLBackend(SDL_Renderer* renderer) : m_renderer(renderer) {}

    void beginFrame() override;
    void render(float deltaTime) override;
    void endFrame() override;

  private:
    void drawRectangle(const Clay_RenderCommand& cmd);
    void drawText(const Clay_RenderCommand& cmd);
    void drawBorder(const Clay_RenderCommand& cmd);
    void drawImage(const Clay_RenderCommand& cmd);
    void startScissors(const Clay_RenderCommand& cmd);
    void endSciccors(const Clay_RenderCommand& cmd);

    void drawRoundedRect(const Clay_RenderCommand& cmd, const SDL_FRect& rect);
    void drawArc(const Clay_RenderCommand& cmd, const SDL_FPoint center, const float radius, const float startAngle, const float endAngle, const float thickness, const Clay_Color color);

    SDL_Renderer* m_renderer;
};
}
