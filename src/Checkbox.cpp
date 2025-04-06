#include "Checkbox.h"
#include "gl_canvas2d.h"

Checkbox::Checkbox(float x, float y, float size, const std::string &label,
                   bool initial)
    : UIElement(x, y, size, size, label), checked(initial), boxSize(size) {}

// Checkbox.cpp
void Checkbox::render() const {
  // Caixa com borda arredondada
  CV::color(0.4f, 0.4f, 0.4f);
  CV::rectFill(x, y, x + boxSize, y + boxSize);

  CV::color(0.7f, 0.7f, 0.7f);
  CV::rect(x, y, x + boxSize, y + boxSize);

  // Checkmark estilizado
  if (checked) {
    CV::color(0.2f, 0.8f, 0.2f);
    CV::line(x + boxSize * 0.2f, y + boxSize * 0.5f, x + boxSize * 0.4f,
             y + boxSize * 0.7f);
    CV::line(x + boxSize * 0.4f, y + boxSize * 0.7f, x + boxSize * 0.8f,
             y + boxSize * 0.3f);
  }

  // Label
  CV::color(0.9f, 0.9f, 0.9f);
  CV::text(x + boxSize + 8, y + boxSize / 2 + 4, label.c_str());
}

bool Checkbox::checkInteraction(int mouseX, int mouseY, int buttonState) {
  bool wasChecked = checked;

  if (buttonState == 0 && // Botão pressionado
      mouseX >= x && mouseX <= x + boxSize && mouseY >= y &&
      mouseY <= y + boxSize) {
    checked = !checked;
  }

  return (checked != wasChecked);
}
