#include "UIManager.h"
#include "Button.h"
#include "Checkbox.h"
#include "Slider.h"

UIManager::UIManager(float panelWidth) : panelWidth(panelWidth) {}

UIManager::~UIManager()
{
    for (auto element : elements) {
        delete element;
    }
}

void UIManager::addButton(float x, float y, float w, float h, const std::string &label, std::function<void()> onClick)
{
    Button *button = new Button(x, y, w, h, label);
    button->setOnClick(onClick);
    elements.push_back(button);
}

void UIManager::addCheckbox(float x, float y, float size, const std::string &label, bool initial,
                            std::function<void(bool)> onChange)
{
    Checkbox *cb = new Checkbox(x, y, size, label, initial);
    // Armazena o estado anterior para detectar mudanças
    cb->setOnClick([cb, onChange]() { onChange(cb->isChecked()); });
    elements.push_back(cb);
}

void UIManager::addSlider(float x, float y, float w, float h, float min, float max, float initial,
                          const std::string &label, std::function<void(float)> onChange)
{
    Slider *slider = new Slider(x, y, w, h, min, max, initial, label, onChange);
    elements.push_back(slider);
}

void UIManager::render()
{
    for (auto element : elements) {
        element->render();
    }
}

bool UIManager::handleMouse(int x, int y, int buttonState)
{
    bool anyInteraction = false;

    // Processa do último para o primeiro (para priorizar elementos no topo)
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        bool interacted = (*it)->checkInteraction(x, y, buttonState);
        if (interacted) {
            anyInteraction = true;
            // Não break para permitir hover em múltiplos elementos
        }
    }

    return anyInteraction;
}
