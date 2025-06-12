#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "Config.h"
#include "UIElement.h"
#include "gl_canvas2d.h"

class Checkbox : public UIElement
{
  private:
    bool checked;
    float boxSize;

  public:
    Checkbox(float x, float y, float size, const std::string &label = "", bool initial = false);

    void render() const override;
    bool checkInteraction(int mouseX, int mouseY, int buttonState) override;
    void setChecked(bool state);
    bool isChecked() const;
};

#endif
