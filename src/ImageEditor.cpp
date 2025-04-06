#include "ImageEditor.h"
#include "Constants.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <string>

ImageEditor::ImageEditor(int width, int height, int panelWidth) {
  uiManager = new UIManager(panelWidth);

  canvasWidth = width;
  canvasHeight = height;
  editorPanelWidth = panelWidth;

  activeLayerIndex = 0;
  drawing = false;
  brushSize = 3;

  // Cor inicial do pincel (vermelho)
  currentColor[0] = 255;
  currentColor[1] = 0;
  currentColor[2] = 0;

  addLayer(); // Adiciona a primeira camada
  initUI();
}

ImageEditor::~ImageEditor() {
  for (auto layer : layers) {
    delete layer;
  }
  delete uiManager;
}

void ImageEditor::initUI() {
  float buttonWidth = editorPanelWidth - 20;
  float buttonHeight = 30;
  float startY = 20;
  float margin = 10;

  // Botão para adicionar layer
  uiManager->addButton(10, startY, buttonWidth, buttonHeight, "Adicionar Layer",
                       [this]() { this->addLayer(); });

  // Botão para remover layer
  uiManager->addButton(10, startY + buttonHeight + margin, buttonWidth,
                       buttonHeight, "Remover Layer", [this]() {
                         if (!layers.empty()) {
                           this->removeLayer(activeLayerIndex);
                         }
                       });
}

void ImageEditor::render() {
  CV::clear(0.8f, 0.8f, 0.8f); // Fundo cinza claro

  renderCheckerBackground();
  renderLayers();
  renderUI();
}

void ImageEditor::renderUI() {
  // Painel lateral
  CV::color(Constants::PANEL_BG_COLOR[0], Constants::PANEL_BG_COLOR[1],
            Constants::PANEL_BG_COLOR[2]);
  CV::rectFill(canvasWidth - editorPanelWidth, 0, canvasWidth, canvasHeight);

  // Renderiza elementos UI
  CV::translate(canvasWidth - editorPanelWidth, 0);
  uiManager->render();

  // Lista de layers
  float listStartY = 150; // Posição mais alta para a lista
  float itemHeight = 30;

  CV::color(Constants::TEXT_COLOR[0], Constants::TEXT_COLOR[1],
            Constants::TEXT_COLOR[2]);
  CV::text(10, listStartY - 20, "Camadas:");

  for (int i = 0; i < layers.size(); i++) {
    float y = listStartY + i * itemHeight;

    // Destaque para layer ativa (mais visível)
    if (i == activeLayerIndex) {
      CV::color(Constants::ACTIVE_LAYER_COLOR[0],
                Constants::ACTIVE_LAYER_COLOR[1],
                Constants::ACTIVE_LAYER_COLOR[2]);
      CV::rectFill(5, y - 5, editorPanelWidth - 5, y + itemHeight - 5);
    }

    // Nome da layer
    CV::color(Constants::TEXT_COLOR[0], Constants::TEXT_COLOR[1],
              Constants::TEXT_COLOR[2]);
    CV::text(10, y + 5, ("Layer " + std::to_string(i + 1)).c_str());

    // Checkbox de visibilidade
    float checkboxSize = 15;
    float checkboxX = editorPanelWidth - 30;
    float checkboxY = y;

    // Caixa do checkbox
    CV::color(Constants::CHECKBOX_COLOR[0], Constants::CHECKBOX_COLOR[1],
              Constants::CHECKBOX_COLOR[2]);
    CV::rectFill(checkboxX, checkboxY, checkboxX + checkboxSize,
                 checkboxY + checkboxSize);
    CV::color(0.7f, 0.7f, 0.7f);
    CV::rect(checkboxX, checkboxY, checkboxX + checkboxSize,
             checkboxY + checkboxSize);

    // Checkmark
    if (layers[i]->isVisible()) {
      CV::color(Constants::CHECKMARK_COLOR[0], Constants::CHECKMARK_COLOR[1],
                Constants::CHECKMARK_COLOR[2]);
      CV::line(checkboxX + 3, checkboxY + checkboxSize / 2,
               checkboxX + checkboxSize / 2, checkboxY + checkboxSize - 3);
      CV::line(checkboxX + checkboxSize / 2, checkboxY + checkboxSize - 3,
               checkboxX + checkboxSize - 3, checkboxY + 3);
    }
  }

  // Informações do pincel
  CV::color(Constants::TEXT_COLOR[0], Constants::TEXT_COLOR[1],
            Constants::TEXT_COLOR[2]);
  CV::text(10, canvasHeight - 60,
           ("Tamanho: " + std::to_string(brushSize)).c_str());

  // Cor atual do pincel
  CV::color(currentColor[0] / 255.0f, currentColor[1] / 255.0f,
            currentColor[2] / 255.0f);
  CV::rectFill(10, canvasHeight - 40, 30, canvasHeight - 20);
  CV::color(0, 0, 0);
  CV::rect(10, canvasHeight - 40, 30, canvasHeight - 20);

  CV::translate(-(canvasWidth - editorPanelWidth), 0);
}

void ImageEditor::renderCheckerBackground() {
  int checkerSize = 10;
  bool colorSwitch = true;

  for (int y = 0; y < canvasHeight; y += checkerSize) {
    for (int x = 0; x < canvasWidth - editorPanelWidth; x += checkerSize) {
      CV::color(colorSwitch ? 0.7f : 0.8f, colorSwitch ? 0.7f : 0.8f,
                colorSwitch ? 0.7f : 0.8f);
      CV::rectFill(x, y, x + checkerSize, y + checkerSize);
      colorSwitch = !colorSwitch;
    }

    if (((canvasWidth - editorPanelWidth) / checkerSize) % 2 == 0) {
      colorSwitch = !colorSwitch;
    }
  }
}

void ImageEditor::handleMouse(int button, int state, int wheel, int direction,
                              int x, int y) {
  // Verifica se está no painel
  if (x > canvasWidth - editorPanelWidth) {
    x -= (canvasWidth - editorPanelWidth); // Ajusta coordenada

    // Área da lista de layers
    float listStartY = 150;
    float itemHeight = 30;
    float listEndY = listStartY + layers.size() * itemHeight;

    // Verifica se clique foi na área da lista
    if (y >= listStartY && y <= listEndY && x >= 5 &&
        x <= editorPanelWidth - 5) {
      if (button == 0 && state == 0) { // Clique com botão esquerdo
        // Calcula qual layer foi clicada
        int clickedLayer = static_cast<int>((y - listStartY) / itemHeight);

        // Verifica se foi no checkbox (área direita)
        if (x >= editorPanelWidth - 30 && x <= editorPanelWidth - 10) {
          toggleLayerVisibility(clickedLayer);
        }
        // Se foi no nome da layer (área esquerda)
        else if (x <= editorPanelWidth - 40) {
          setActiveLayer(clickedLayer);
        }
      }
    }

    uiManager->handleMouse(x, y, state);
    return;
  }

  // Manipulação do canvas
  if (button == 0) {  // Botão esquerdo
    if (state == 0) { // Pressionado
      drawing = true;
      if (activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
        layers[activeLayerIndex]->setDrawingColor(
            currentColor[0], currentColor[1], currentColor[2]);
        layers[activeLayerIndex]->drawPixel(x, y, brushSize);
      }
    } else { // Solto
      drawing = false;
    }
  }

  if (drawing && activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
    layers[activeLayerIndex]->drawPixel(x, y, brushSize);
  }
}

void ImageEditor::handleKeyboard(unsigned char key) {
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
  case '1':
    currentColor[0] = 255;
    currentColor[1] = 0;
    currentColor[2] = 0; // Vermelho
    break;
  case '2':
    currentColor[0] = 0;
    currentColor[1] = 255;
    currentColor[2] = 0; // Verde
    break;
  case '3':
    currentColor[0] = 0;
    currentColor[1] = 0;
    currentColor[2] = 255; // Azul
    break;
  case '+':
    brushSize = std::min(brushSize + 1, 20);
    break;
  case '-':
    brushSize = std::max(brushSize - 1, 1);
    break;
  }
}

void ImageEditor::handleKeyboardUp(unsigned char key) {
  // Implementação para eventos de teclado liberado
}

void ImageEditor::addLayer() {
  Layer *newLayer = new Layer(canvasWidth, canvasHeight);
  layers.push_back(newLayer);
  setActiveLayer(layers.size() - 1);
}

void ImageEditor::removeLayer(int index) {
  if (index < 0 || index >= layers.size()) {
    return;
  }

  if (layers.size() <= 1) {
    return; // Não remove a última camada
  }

  delete layers[index];
  layers.erase(layers.begin() + index);

  // Ajusta o índice ativo
  if (activeLayerIndex >= index) {
    activeLayerIndex = std::max(0, activeLayerIndex - 1);
  }
}

void ImageEditor::setActiveLayer(int index) {
  if (index >= 0 && index < layers.size()) {
    // Desativa a camada atual
    if (activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
      layers[activeLayerIndex]->setActive(false);
    }

    // Ativa a nova camada
    layers[index]->setActive(true);
    activeLayerIndex = index;
  }
}

bool ImageEditor::fileExists(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file) {
    fclose(file);
    return true;
  }
  return false;
}

void ImageEditor::loadImageToLayer(int layerIndex, const char *filename) {
  if (!fileExists(filename) || layerIndex < 0 || layerIndex >= layers.size()) {
    return;
  }
  layers[layerIndex]->loadImage(filename);
}

void ImageEditor::renderLayers() {
  for (auto layer : layers) {
    if (layer->isVisible()) {
      layer->render();
    }
  }
}

void ImageEditor::moveLayerUp(int index) {
  if (index > 0 && index < layers.size()) {
    std::swap(layers[index], layers[index - 1]);
    if (activeLayerIndex == index) {
      activeLayerIndex--;
    }
  }
}

void ImageEditor::moveLayerDown(int index) {
  if (index >= 0 && index < layers.size() - 1) {
    std::swap(layers[index], layers[index + 1]);
    if (activeLayerIndex == index) {
      activeLayerIndex++;
    }
  }
}

void ImageEditor::toggleLayerVisibility(int index) {
  if (index >= 0 && index < layers.size()) {
    layers[index]->setVisible(!layers[index]->isVisible());
  }
}
