#pragma once

#include <SDL3/SDL_video.h>

namespace slay {

struct AppConfig;

struct WindowSize {
    float width;
    float height;
};

class Platform {
    public:
    ~Platform();
    bool init(const AppConfig& config);

    void pollEvents();
    WindowSize getWindowSize() const;
    SDL_Window* getWindow() const {return m_window;}
    bool shouldQuit() const {return m_shouldQuit;}
    bool isWindowResized() const {return m_isWindowResized;}

    private:
    bool m_shouldQuit = false;
    bool m_isWindowResized = false;
    SDL_Window* m_window = nullptr;
};
}
