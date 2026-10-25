#include "hello_app.h"

#include "slay/app.h"
#include "slay/layout_engine.h"

#include <SDL3/SDL_log.h>

HelloApp::HelloApp(slay::AppConfig& config) : slay::App(config) {};

void HelloApp::onStart() {
    SDL_Log("Example sucsesfully started!");
    loadFont("/Inter-Regular.otf");
}

void HelloApp::onRender() {
    CLAY(CLAY_ID("OuterContainer"), {.layout = {
                                         .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                                         .padding = CLAY_PADDING_ALL(8),
                                         .childGap = 16,
                                         .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                     }}) {
        CLAY(CLAY_ID("SideBar"),
             {
                 .layout =
                     {
                         .sizing = {CLAY_SIZING_FIXED(150), CLAY_SIZING_GROW()},
                         .layoutDirection = CLAY_TOP_TO_BOTTOM,
                     },
                 .backgroundColor = {140, 140, 140, 255},
                 .cornerRadius = CLAY_CORNER_RADIUS(5),
                 .border = {.color = {180, 80, 80, 255}, .width = CLAY_BORDER_OUTSIDE(2)},

             }) {
            CLAY_TEXT(CLAY_STRING("WORKING RHAAAAA!!!!!!!!!!!"),
                      CLAY_TEXT_CONFIG({.textColor = {0, 0, 0, 255}, .fontId = 1, .fontSize = 24}));
        }
    }
}

void HelloApp::onShutDown() {
    SDL_Log("Example sucsesfully Ended!");
}
