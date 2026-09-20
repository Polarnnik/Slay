#include "slay/app.h"
#include "slay/font_manager.h"
#include "slay/layout_engine.h"
#include "slay/platform.h"
#include "slay/renderer/sdl_renderer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_filesystem.h>
#include <memory>


slay::App::App(const slay::AppConfig &config) {
    m_config = config;
}

bool slay::App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init()) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!m_platform.init(m_config))
        return false;
    if (!m_layout.init(m_config.width, m_config.height))
        return false;
    m_layout.setFontManager(&m_fontManager);

    m_renderer = std::make_unique<SDLRenderer>(m_platform.getWindow(), m_fontManager, m_config.vsync);
    if (!m_renderer->init())
        return false;
    return true;
}

slay::FontId slay::App::loadFont(const std::string& filepath) {
    std::string fullPath = SDL_GetBasePath() + filepath;
    return m_fontManager.load(fullPath);
}

void slay::App::run() {
    Uint64 lastTime = SDL_GetTicksNS();

    onStart();
    while(!m_platform.shouldQuit()) {
        Uint64 currentTime = SDL_GetTicksNS();
        float deltaTime = static_cast<float>(currentTime - lastTime) / 1'000'000'000.0f;
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.1f);

        m_platform.pollEvents();
        if (m_platform.isWindowResized()) {
            WindowSize size = m_platform.getWindowSize();
            Clay_SetLayoutDimensions({.width = size.width, .height = size.height});
        }

        m_layout.begin();
        onRender();
        RenderCommandArray command = m_layout.end(deltaTime);

        m_renderer->beginFrame();
        m_renderer->render(command);
        m_renderer->endFrame();
    }
    onShutDown();
    shutdown();
}

void slay::App::shutdown() {
    if (m_renderer) m_renderer->shutdown();
    m_fontManager.clear();
    m_platform.shutdown();
}

slay::App::~App() {
    TTF_Quit();
    SDL_Quit();
}
