#pragma once
#include "slay/font_manager.h"
#include "slay/renderer/i_renderer.h"
#include "slay/layout_engine.h"
#include "slay/platform.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <string>

namespace slay {

struct AppConfig {
    std::string title;
    int width = 800;
    int height = 600;
    bool vsync = true;
};

class App {
  public:
    explicit App(const AppConfig& config);
    ~App();

    void run();
    bool init();
    void shutdown();

  protected:
    virtual void onStart() {};
    virtual void onRender() {};
    virtual void onShutDown() {};
    FontId loadFont(const std::string& filepath);

  private:
    AppConfig m_config;

    std::unique_ptr<IRenderer> m_renderer;
    FontManager m_fontManager;
    LayoutEngine m_layout;
    Platform m_platform;
};
}
