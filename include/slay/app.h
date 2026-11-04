#pragma once
#include "slay/font_manager.h"
#include "slay/layout_engine.h"
#include "slay/platform.h"
#include "slay/renderer/i_renderer.h"
#include "slay/ui/types.h"
#include "slay/ui/ui.h"
#include "slay/view.h"

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

    void run(View& view);
    bool init();
    void shutdown();

    FontId loadFont(const std::string& filepath);
    void setDefaultFont(FontId f) { m_ui.setDefaultFont(f); }

  private:
    AppConfig m_config;

    Ui m_ui;
    std::unique_ptr<IRenderer> m_renderer;
    FontManager m_fontManager;
    LayoutEngine m_layout;
    Platform m_platform;
};
} // namespace slay
