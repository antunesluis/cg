#include "Button.h"
#include "gl_canvas2d.h"

Button::Button(float x, float y, float w, float h, const std::string &label)
    : UIElement(x, y, w, h, label) {}

void Button::render() const {
  // Fundo com gradiente sutil
  if (pressed) {
    CV::color(0.2f, 0.5f, 0.8f);
  } else if (hovered) {
    CV::color(config::colors::button_hover[0], config::colors::button_hover[1],
              config::colors::button_hover[2]);
  } else {
    CV::color(0.25f, 0.25f, 0.3f);
  }
  CV::rectFill(x, y, x + width, y + height);

  // Borda sutil
  CV::color(0.4f, 0.4f, 0.5f);
  CV::rect(x, y, x + width, y + height);

  // Texto centralizado com sombra
  CV::color(0.0f, 0.0f, 0.0f);
  CV::text(x + (width - label.length() * 8) / 2 + 1, y + height / 2 + 5,
           label.c_str());

  CV::color(1.0f, 1.0f, 1.0f);
  CV::text(x + (width - label.length() * 8) / 2, y + height / 2 + 4,
           label.c_str());
}

bool Button::checkInteraction(int mouseX, int mouseY, int buttonState) {
  hovered = (mouseX >= x && mouseX <= x + width && mouseY >= y &&
             mouseY <= y + height);

  if (buttonState == 0) { // Botão pressionado
    pressed = hovered;
  } else if (buttonState == 1 && pressed && hovered) { // Botão solto
    if (onClickCallback) {
      onClickCallback();
    }
    pressed = false;
  }

  return hovered || pressed;
}
