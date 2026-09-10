#include "slay/platform.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "slay/app.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

bool slay::Platform::init(const AppConfig& config) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(
        config.title.c_str(),
        config.width,
        config.height,
        SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void slay::Platform::pollEvents() {
    SDL_Event event;
    m_isWindowResized = false;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                m_shouldQuit = true;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                m_isWindowResized = true;
                break;
        }
    }
}

slay::WindowSize slay::Platform::getWindowSize() const {
    int width, height = 0;

    SDL_GetWindowSize(m_window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

slay::Platform::~Platform() {
    if (m_window) {
            SDL_DestroyWindow(m_window);
        }

        SDL_Quit();
}
