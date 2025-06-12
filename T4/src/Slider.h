// Slider.h
#ifndef SLIDER_H
#define SLIDER_H

#include "UIElement.h"
#include "gl_canvas2d.h"
#include <functional>

class Slider : public UIElement
{
  private:
    float value;
    float min, max;
    bool dragging;
    float sliderWidth;
    std::string label;
    std::function<void(float)> onChangeCallback;

  public:
    Slider(float x, float y, float w, float h, float minVal, float maxVal, float initialVal, const std::string &lbl,
           std::function<void(float)> callback);

    void render() const override;
    bool checkInteraction(int mouseX, int mouseY, int buttonState) override;
    float getValue() const;
    void setValue(float val);
};

#endif
