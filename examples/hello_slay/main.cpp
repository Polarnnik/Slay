#include "slay/app.h"

#include "hello_app.h"

int main() {
    slay::AppConfig config = {
        .title = "Hello slay",
        .width = 800,
        .height = 600,
    };

    HelloApp app(config);
    if (!app.init())
        return -1;
    app.run();
}
