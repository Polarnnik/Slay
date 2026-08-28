#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <string>
#include <vector>

namespace slay {

struct AppConfig {
    std::string title;
    int width = 800;
    int height = 600;
};

class App {
  public:
    explicit App(const AppConfig& config);
    virtual ~App();

    void run();
    bool init();
    SDL_Window* getWindow() const {return m_window;}
    SDL_Renderer* getRendere() const {return m_renderer;}

  protected:
    virtual void onStart() {};
    virtual void onRender() {};
    virtual void onShutDown() {};

  private:
    bool initClay(const AppConfig& config);
    bool initSDL(const AppConfig& config);

    AppConfig m_config;

    bool m_isRunning = false;
    std::vector<char> m_clayArena;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};


}
