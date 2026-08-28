#include "slay/app.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
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
    return true;
}

void slay::App::run() {
    m_isRunning = true;

    onStart();
    SDL_Event event;
    while(m_isRunning) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                m_isRunning = false;
            }
        }
        onRender();

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
