#include "Checkbox.h"

Checkbox::Checkbox(float x, float y, float size, const std::string &label, bool initial)
    : UIElement(x, y, size, size, label), checked(initial), boxSize(size)
{
}

void Checkbox::render() const
{
    // Caixa do checkbox (cinza escuro)
    CV::color(0.4f, 0.4f, 0.4f);
    CV::rectFill(x, y, x + boxSize, y + boxSize);

    // Borda (branco quase puro)
    CV::color(0.9f, 0.9f, 0.9f);
    CV::rect(x, y, x + boxSize, y + boxSize);

    // Checkmark (verde)
    if (checked) {
        CV::color(0.2f, 0.8f, 0.2f);
        CV::line(x + 3, y + boxSize / 2, x + boxSize / 2, y + boxSize - 3);
        CV::line(x + boxSize / 2, y + boxSize - 3, x + boxSize - 3, y + 3);
    }

    // Label (branco quase puro)
    if (!label.empty()) {
        CV::color(0.9f, 0.9f, 0.9f);
        CV::text(x + boxSize + 5, y + boxSize / 2 + 4, label.c_str());
    }
}

bool Checkbox::checkInteraction(int mouseX, int mouseY, int buttonState)
{
    bool wasChecked = checked;
    hovered = isMouseOver(mouseX, mouseY);

    if (buttonState == 0 && hovered) { // Botão pressionado
        pressed = true;
    } else if (buttonState == 1 && pressed && hovered) { // Botão solto
        checked = !checked;
        printf("Checkbox clicado! Novo estado: %d\n", checked);
        if (onClickCallback) {
            onClickCallback(); // Dispara o callback
        }
    }

    pressed = (buttonState == 0) ? pressed : false;
    return (checked != wasChecked);
}

void Checkbox::setChecked(bool state) { checked = state; }
bool Checkbox::isChecked() const { return checked; }
