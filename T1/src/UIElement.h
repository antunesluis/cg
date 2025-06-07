#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "gl_canvas2d.h"
#include <functional>
#include <string>

class UIElement
{
  protected:
    float x, y, width, height;
    bool hovered = false;
    bool pressed = false;
    std::string label;
    std::function<void()> onClickCallback;

  public:
    UIElement(float x, float y, float w, float h, const std::string &label)
        : x(x), y(y), width(w), height(h), label(label)
    {
    }

    virtual ~UIElement() = default;

    // Métodos virtuais puros (obrigatórios para classes derivadas)
    virtual void render() const = 0;
    virtual bool checkInteraction(int mouseX, int mouseY, int buttonState) = 0;

    // Métodos implementados inline (não precisam de .cpp)
    void setOnClick(std::function<void()> callback) { onClickCallback = callback; }

    bool isHovered() const { return hovered; }
    bool isPressed() const { return pressed; }
};

#endif
