#ifndef BUTTON_H
#define BUTTON_H

#include "UIElement.h"

class Button : public UIElement {
private:
  float color[3] = {0.4f, 0.4f, 0.4f};
  float hoverColor[3] = {0.6f, 0.6f, 0.6f};
  float pressColor[3] = {0.3f, 0.3f, 0.3f};

public:
  Button(float x, float y, float w, float h, const std::string &label);
  void render() const override;
  bool checkInteraction(int mouseX, int mouseY, int buttonState) override;
  void setColors(float normal[3], float hover[3], float press[3]);
};

#endif
