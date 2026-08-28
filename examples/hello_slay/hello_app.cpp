#include "hello_app.h"
#include "slay/app.h"
#include <SDL3/SDL_log.h>

HelloApp::HelloApp(slay::AppConfig& config) : slay::App(config) {};

void HelloApp::onStart() {
    SDL_Log("Example sucsesfully started!");
}

void HelloApp::onRender() {
    SDL_Renderer* renderer = getRendere();

    SDL_FRect buttonBox {
        100.0f,
        100.0f,
        200.0f,
        60.0f,
    };

    SDL_SetRenderDrawColor(renderer, 90, 130, 255, 255);
    SDL_RenderFillRect(renderer, &buttonBox);
}

void HelloApp::onShutDown() {
    SDL_Log("Example sucsesfully Ended!");
}
