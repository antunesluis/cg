#include "Checkbox.h"

Checkbox::Checkbox(float x, float y, float size, const std::string &label, bool initial)
    : UIElement(x, y, size, size, label), checked(initial), boxSize(size)
{
}

void Checkbox::render() const
{
    // Caixa do checkbox
    CV::color(config::colors::checkbox[0], config::colors::checkbox[1], config::colors::checkbox[2]);
    CV::rectFill(x, y, x + boxSize, y + boxSize);

    // Borda
    CV::color(config::colors::checkbox_border[0], config::colors::checkbox_border[1],
              config::colors::checkbox_border[2]);
    CV::rect(x, y, x + boxSize, y + boxSize);

    // Checkmark
    if (checked) {
        CV::color(config::colors::checkmark[0], config::colors::checkmark[1], config::colors::checkmark[2]);
        CV::line(x + 3, y + boxSize / 2, x + boxSize / 2, y + boxSize - 3);
        CV::line(x + boxSize / 2, y + boxSize - 3, x + boxSize - 3, y + 3);
    }

    // Label (se existir)
    if (!label.empty()) {
        CV::color(config::colors::checkbox_border[0], config::colors::checkbox_border[1],
                  config::colors::checkbox_border[2]);
        CV::text(x + boxSize + 5, y + boxSize / 2 + 4, label.c_str());
    }
}

bool Checkbox::checkInteraction(int mouseX, int mouseY, int buttonState)
{
    bool wasChecked = checked;

    if (buttonState == 0 && // Botão pressionado
        mouseX >= x && mouseX <= x + boxSize && mouseY >= y && mouseY <= y + boxSize) {
        checked = !checked;
    }

    return (checked != wasChecked);
}

void Checkbox::setChecked(bool state) { checked = state; }
bool Checkbox::isChecked() const { return checked; }
