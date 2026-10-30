#pragma once

namespace slay {

class Element {
  public:
    Element() = default;
    Element(const Element&) = delete;
    Element(Element&&) = default;
    Element& operator=(const Element&) = delete;
    Element& operator=(Element&&) = delete;
    virtual void emit() = 0;

  protected:
    ~Element() = default;
};

} // namespace slay
