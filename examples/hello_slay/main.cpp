#include "slay/app.h"
#include "slay/ui/element.h"
#include "slay/ui/types.h"
#include "slay/ui/ui.h"
#include "slay/view.h"

class MainView : public slay::View {
  public:
    slay::Element* render(slay::Ui& ui) override {
        return ui.div().bg(slay::Color::rgba(0x00A000ff)).child(ui.text("WORKS RHAAAAAA!!!"));
    }
};

int main() {
    slay::AppConfig config = {
        .title = "Hello slay",
        .width = 800,
        .height = 600,
    };

    slay::App app(config);

    MainView view;

    if (!app.init())
        return -1;
    auto font = app.loadFont("./Inter-Regular.otf");
    app.setDefaultFont(font);
    app.run(view);
}
