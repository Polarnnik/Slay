#include "slay/app.h"
#include "slay/backend/sdl_backend.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_timer.h>
#include <memory>

#define CLAY_IMPLEMENTATION
#include <clay.h>

void clayError(Clay_ErrorData errorData) {
    SDL_Log("%s", errorData.errorText.chars);
}

slay::App::App(const slay::AppConfig &config) {
    m_config = config;
}

slay::App::~App() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool slay::App::init() {
    if (!initSDL(m_config))
        return false;
    if (!initClay(m_config))
        return false;

    m_backend = std::make_unique<SDLBackend>(m_renderer);
    return true;
}

void slay::App::run() {
    m_isRunning = true;
    Uint64 lastTime = SDL_GetTicksNS();

    onStart();
    SDL_Event event;
    while(m_isRunning) {
        Uint64 currentTime = SDL_GetTicksNS();
        float deltaTime = static_cast<float>(currentTime - lastTime) / 1'000'000'000.0f;
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.1f);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                m_isRunning = false;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                int width, height = 0;
                SDL_GetWindowSize(m_window, &width, &height);
                Clay_SetLayoutDimensions({static_cast<float>(width), static_cast<float>(height)});
            }
        }
        m_backend->beginFrame();
        onRender();
        m_backend->render(deltaTime);
        m_backend->endFrame();
    }
    onShutDown();
}

bool slay::App::initSDL(const AppConfig &config) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(config.title.c_str(), config.width, config.height,
                                SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        SDL_Log("Failed to create SDL window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_backend = std::make_unique<SDLBackend>(m_renderer);

    return true;
}

bool slay::App::initClay(const AppConfig& config) {
  uint64_t totalMemorySize = Clay_MinMemorySize();
  m_clayArena.resize(totalMemorySize);
  Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, m_clayArena.data());
  if (!Clay_Initialize(arena, {static_cast<float>(config.width), static_cast<float>(config.height)}, {clayError, nullptr}))
      return false;
  return true;
}
