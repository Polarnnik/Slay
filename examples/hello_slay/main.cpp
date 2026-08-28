#include "hello_app.h"
#include "slay/app.h"

int main() {
    slay::AppConfig config = {
        .title = "Hello saly",
        .width = 800,
        .height = 600,
    };

    HelloApp app(config);
    if (!app.init()) return -1;
    app.run();

}
