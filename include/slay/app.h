#pragma once

#include <string>
namespace slay {

struct AppConfig {
    std::string title;
    int width = 800;
    int height = 600;
};

class App {
  public:
    explicit App(const AppConfig& config);
};


}
