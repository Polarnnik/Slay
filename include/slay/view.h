#pragma once

#include "slay/ui/ui.h"

namespace slay {

    class View {
        public:
            virtual ~View() = default;
            virtual Element* render(Ui& ui) = 0;
    };
}
