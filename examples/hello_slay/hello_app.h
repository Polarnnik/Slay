#pragma once

#include <slay/app.h>

class HelloApp final : public slay::App {
  public:
    HelloApp(slay::AppConfig& config);
  protected:
    void onStart() override;
    void onRender() override;
    void onShutDown() override;
};
