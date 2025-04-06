#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
// Cores temáticas
constexpr float PANEL_BG_COLOR[3] = {0.15f, 0.15f, 0.17f};
constexpr float ACTIVE_LAYER_COLOR[3] = {0.2f, 0.4f, 0.8f};
constexpr float TEXT_COLOR[3] = {0.9f, 0.9f, 0.9f};
constexpr float CHECKBOX_COLOR[3] = {0.4f, 0.4f, 0.4f};
constexpr float CHECKMARK_COLOR[3] = {0.2f, 0.8f, 0.2f};

// Dimensões e espaçamentos
constexpr float BUTTON_START_Y = 20.0f;
constexpr float BUTTON_HEIGHT = 30.0f;
constexpr float BUTTON_MARGIN = 10.0f;
constexpr float LAYER_LIST_START_Y = 150.0f;
constexpr float LAYER_ITEM_HEIGHT = 30.0f;
constexpr float CHECKBOX_SIZE = 15.0f;

// Configurações do pincel
constexpr int DEFAULT_BRUSH_SIZE = 3;
constexpr int MAX_BRUSH_SIZE = 20;
constexpr int MIN_BRUSH_SIZE = 1;

// Dimensões da tela e painel do editor
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int EDITOR_PANEL_WIDTH = 280;
} // namespace Constants

#endif // CONSTANTS_H
