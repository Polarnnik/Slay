#include "hello_app.h"
#include "slay/app.h"
#include "slay/backend/i_backend.h"
#include <SDL3/SDL_log.h>

HelloApp::HelloApp(slay::AppConfig& config) : slay::App(config) {};

void HelloApp::onStart() {
    SDL_Log("Example sucsesfully started!");
}

void HelloApp::onRender() {
    SDL_Renderer* renderer = getRendere();

    SDL_SetRenderDrawColor(renderer, 90, 130, 255, 255);
    CLAY(CLAY_ID("OuterContainer"), {
        .layout = {
            .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
            .padding = CLAY_PADDING_ALL(8),
            .childGap = 16,
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        }
    }) {
        CLAY(CLAY_ID("SideBar"), {
            .layout = {
                .sizing = {CLAY_SIZING_FIXED(150), CLAY_SIZING_GROW()},
                .layoutDirection = CLAY_TOP_TO_BOTTOM,},
            .backgroundColor = {140, 140, 140, 255}
        }){}
    }
}

void HelloApp::onShutDown() {
    SDL_Log("Example sucsesfully Ended!");
}
