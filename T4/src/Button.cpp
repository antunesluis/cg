#include "Button.h"
#include "gl_canvas2d.h"

Button::Button(float x, float y, float w, float h, const std::string &label) : UIElement(x, y, w, h, label) {}

void Button::render() const
{
    // Fundo com gradiente sutil
    if (pressed) {
        CV::color(0.2f, 0.5f, 0.8f);
    } else if (hovered) {
        CV::color(0.3f, 0.3f, 0.4f);
    } else {
        CV::color(0.25f, 0.25f, 0.3f);
    }
    CV::rectFill(x, y, x + width, y + height);

    // Borda sutil
    CV::color(0.4f, 0.4f, 0.5f);
    CV::rect(x, y, x + width, y + height);

    // Texto centralizado com sombra
    CV::color(0.0f, 0.0f, 0.0f);
    CV::text(x + (width - label.length() * 10) / 2 + 1, y + height / 2 + 5, label.c_str());

    CV::color(1.0f, 1.0f, 1.0f);
    CV::text(x + (width - label.length() * 10) / 2, y + height / 2 + 4, label.c_str());
}

bool Button::checkInteraction(int mouseX, int mouseY, int buttonState)
{
    hovered = isMouseOver(mouseX, mouseY);

    if (buttonState == 0) { // Botão pressionado
        if (hovered) {
            pressed = true;
            wasPressed = true;
        }
    } else if (buttonState == 1) { // Botão solto
        if (pressed && hovered && wasPressed && onClickCallback) {
            onClickCallback();
        }
        pressed = false;
        wasPressed = false;
    }

    return hovered || pressed;
}
