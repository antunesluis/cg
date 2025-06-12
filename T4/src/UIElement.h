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
    bool wasPressed = false; // Para detectar click release
    std::string label;
    std::function<void()> onClickCallback;

  public:
    UIElement(float x, float y, float w, float h, const std::string &label)
        : x(x), y(y), width(w), height(h), label(label)
    {
    }

    virtual ~UIElement() = default;

    // Métodos virtuais puros
    virtual void render() const = 0;
    virtual bool checkInteraction(int mouseX, int mouseY, int buttonState) = 0;

    // Métodos implementados
    void setOnClick(std::function<void()> callback) { onClickCallback = callback; }

    bool isHovered() const { return hovered; }
    bool isPressed() const { return pressed; }

    // Método para verificar se o mouse está sobre o elemento
    bool isMouseOver(int mouseX, int mouseY) const
    {
        return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
    }
};

#endif
