#include "slay/ui/div.h"
#include "slay/ui/text.h"

#include <clay.h>

#include "src/ui/clay_convert.h"

void slay::DivElement::emit() {
    Clay__OpenElement();
    Clay__ConfigureOpenElement(toClay(m_style));
    for (Element* e : m_children)
        e->emit();
    Clay__CloseElement();
}

void slay::TextElement::emit() {
    Clay__OpenTextElement({false, m_str.len, m_str.data}, toClay(m_textConf));
}
