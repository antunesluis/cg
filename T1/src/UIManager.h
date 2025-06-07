#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "UIElement.h"
#include <vector>

class UIManager
{
  private:
    std::vector<UIElement *> elements;
    float panelWidth;

  public:
    UIManager(float panelWidth);
    ~UIManager(); // Precisa limpar os ponteiros

    void addButton(float x, float y, float w, float h, const std::string &label, std::function<void()> onClick);
    void addCheckbox(float x, float y, float size, const std::string &label, bool initial,
                     std::function<void(bool)> onChange);
    void addSlider(float x, float y, float w, float h, float min, float max, float initial, const std::string &label,
                   std::function<void(float)> onChange);

    void render();
    void handleMouse(int x, int y, int buttonState);
};

#endif
