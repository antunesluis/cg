// Slider.cpp
#include "Slider.h"

Slider::Slider(float x, float y, float w, float h, float minVal, float maxVal, float initialVal, const std::string &lbl,
               std::function<void(float)> callback)
    : UIElement(x, y, w, h, lbl), min(minVal), max(maxVal), value(initialVal), label(lbl), onChangeCallback(callback),
      sliderWidth(w - 40), dragging(false)
{
}

void Slider::render() const
{
    // Fundo do slider
    CV::color(0.3f, 0.3f, 0.35f);
    CV::rectFill(x, y, x + width, y + height);

    // Barra do slider
    CV::color(0.5f, 0.5f, 0.55f);
    CV::rectFill(x + 20, y + height / 2 - 5, x + 20 + sliderWidth, y + height / 2 + 5);

    // Alça do slider
    float handlePos = 20 + ((value - min) / (max - min)) * sliderWidth;
    CV::color(0.2f, 0.6f, 0.8f);
    CV::rectFill(x + handlePos - 5, y, x + handlePos + 5, y + height);

    // Texto e valor
    CV::color(1, 1, 1);
    CV::text(x + 5, y + height / 2 - 10, label.c_str());
    CV::text(x + width - 35, y + height / 2 - 10, std::to_string(value).substr(0, 4).c_str());
}

bool Slider::checkInteraction(int mouseX, int mouseY, int buttonState)
{
    bool wasDragging = dragging;

    if (buttonState == 0) { // Botão pressionado
        if (mouseX >= x + 15 && mouseX <= x + 25 + sliderWidth && mouseY >= y && mouseY <= y + height) {
            dragging = true;
        }
    } else if (buttonState == 1) { // Botão solto
        dragging = false;
    }

    if (dragging) {
        float newPos = mouseX - x - 20;
        newPos = std::max(0.0f, std::min(sliderWidth, newPos));
        float newValue = min + (newPos / sliderWidth) * (max - min);

        if (newValue != value) {
            value = newValue;
            if (onChangeCallback) {
                onChangeCallback(value);
            }
        }
    }

    return dragging || wasDragging;
}

float Slider::getValue() const { return value; }

void Slider::setValue(float val) { value = std::min(max, std::max(min, val)); }
