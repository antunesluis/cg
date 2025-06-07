#ifndef BUTTON_H
#define BUTTON_H

#include "Config.h"
#include "UIElement.h"

class Button : public UIElement
{

  public:
    Button(float x, float y, float w, float h, const std::string &label);
    void render() const override;
    bool checkInteraction(int mouseX, int mouseY, int buttonState) override;
    void setColors(float normal[3], float hover[3], float press[3]);
};

#endif
