#pragma once
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
};

class App {
  public:
    explicit App(const AppConfig& config);

    void run();
    bool init();

  protected:
    virtual void onStart() {};
    virtual void onRender() {};
    virtual void onShutDown() {};

  private:
    AppConfig m_config;

    std::unique_ptr<IRenderer> m_renderer;
    LayoutEngine m_layout;
    Platform m_platform;
};


}
