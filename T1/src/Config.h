// Constants.h
#pragma once
#include <array>

namespace config
{
namespace screen
{
constexpr int width = 1280;
constexpr int height = 1000;
constexpr float aspect_ratio = width / static_cast<float>(height);
} // namespace screen

namespace editor
{
constexpr int panel_width = 280;
constexpr int max_layers = 32;
} // namespace editor

namespace colors
{
constexpr std::array<float, 3> panel_bg = {0.15f, 0.15f, 0.17f};
constexpr std::array<float, 3> active_layer = {0.2f, 0.4f, 0.8f};
constexpr std::array<float, 3> text = {0.9f, 0.9f, 0.9f};
constexpr std::array<float, 3> checkbox = {0.4f, 0.4f, 0.4f};
constexpr std::array<float, 3> checkbox_border = {0.9f, 0.9f, 0.9f};
constexpr std::array<float, 3> checkmark = {0.2f, 0.8f, 0.2f};
constexpr std::array<float, 3> button_normal = {0.25f, 0.25f, 0.3f};
constexpr std::array<float, 3> button_hover = {0.3f, 0.3f, 0.4f};
constexpr std::array<float, 3> button_pressed = {0.2f, 0.5f, 0.8f};
constexpr std::array<float, 3> button_border = {0.4f, 0.4f, 0.5f};
constexpr std::array<float, 3> button_text = {1.0f, 1.0f, 1.0f};
constexpr std::array<float, 3> button_text_shadow = {0.0f, 0.0f, 0.0f};

constexpr std::array<std::array<float, 3>, 16> colorPickerColors = {
    // Primeira linha (cores básicas)
    std::array<float, 3>{1.0f, 0.0f, 0.0f}, // Vermelho
    std::array<float, 3>{0.0f, 1.0f, 0.0f}, // Verde
    std::array<float, 3>{0.0f, 0.0f, 1.0f}, // Azul
    std::array<float, 3>{0.0f, 0.0f, 0.0f}, // Preto
    std::array<float, 3>{1.0f, 1.0f, 0.0f}, // Amarelo
    std::array<float, 3>{1.0f, 0.0f, 1.0f}, // Magenta
    std::array<float, 3>{0.0f, 1.0f, 1.0f}, // Ciano
    std::array<float, 3>{1.0f, 1.0f, 1.0f}, // Branco

    // Segunda linha (tons pastel e secundários)
    std::array<float, 3>{1.0f, 0.5f, 0.5f},  // Rosa claro
    std::array<float, 3>{0.5f, 1.0f, 0.5f},  // Verde claro
    std::array<float, 3>{0.5f, 0.5f, 1.0f},  // Azul claro
    std::array<float, 3>{0.5f, 0.5f, 0.5f},  // Cinza
    std::array<float, 3>{1.0f, 0.65f, 0.0f}, // Laranja
    std::array<float, 3>{0.5f, 0.0f, 0.5f},  // Roxo
    std::array<float, 3>{0.0f, 0.5f, 0.5f},  // Turquesa
    std::array<float, 3>{0.6f, 0.3f, 0.0f}   // Marrom
};
} // namespace colors

namespace layout
{
constexpr float button_start_y = 20.0f; // Posição inicial dos botões
constexpr float button_height = 30.0f;  // Altura dos botões
constexpr float button_margin = 10.0f;  // Espaço entre botões
constexpr float layer_list_start_y = 260.0f;
constexpr float layer_item_height = 25.0f;
constexpr float effects_start_y = 460.0f;
constexpr float checkbox_size = 15.0f;
constexpr int side_margin = 10;

constexpr float color_picker_start_y = 860.0f; // Posição Y do color picker
constexpr float color_box_size = 25.0f;        // Tamanho de cada quadrado de cor
constexpr float color_box_margin = 5.0f;       // Margem entre os quadrados

} // namespace layout

namespace brush
{
constexpr int default_size = 3;
constexpr int max_size = 20;
constexpr int min_size = 1;
} // namespace brush

} // namespace config
