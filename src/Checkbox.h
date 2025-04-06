#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "UIElement.h"

class Checkbox : public UIElement {
private:
  bool checked;
  float boxSize;

public:
  Checkbox(float x, float y, float size, const std::string &label,
           bool initial = false);
  void render() const override;
  bool checkInteraction(int mouseX, int mouseY, int buttonState) override;
  bool isChecked() const { return checked; }
  void setChecked(bool state) { checked = state; }
};

#endif
