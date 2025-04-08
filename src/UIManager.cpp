#include "UIManager.h"
#include "Button.h"
#include "Checkbox.h"

UIManager::UIManager(float panelWidth) : panelWidth(panelWidth) {}

UIManager::~UIManager() {
  for (auto element : elements) {
    delete element; // Limpeza manual
  }
}

void UIManager::addButton(float x, float y, float w, float h,
                          const std::string &label,
                          std::function<void()> onClick) {
  Button *button = new Button(x, y, w, h, label);
  button->setOnClick(onClick);
  elements.push_back(button);
}

void UIManager::addCheckbox(float x, float y, float size,
                            const std::string &label, bool initial,
                            std::function<void(bool)> onChange) {
  Checkbox *cb = new Checkbox(x, y, size, label, initial);
  cb->setOnClick([cb, onChange]() { onChange(cb->isChecked()); });
  elements.push_back(cb);
}

void UIManager::render() {
  for (auto element : elements) {
    element->render();
  }
}

void UIManager::handleMouse(int x, int y, int buttonState) {
  for (auto element : elements) {
    element->checkInteraction(x, y, buttonState);
  }
}
