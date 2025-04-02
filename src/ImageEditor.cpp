#include "ImageEditor.h"
#include "gl_canvas2d.h"
#include <string>

ImageEditor::ImageEditor(int width, int height, int panelWidth) {
  canvasWidth = width;
  canvasHeight = height;
  editorPanelWidth = panelWidth;
  activeLayerIndex = 0;

  // Campos de controle de desenho
  drawing = false;
  brushSize = 3;

  addLayer();
}

ImageEditor::~ImageEditor() {
  for (auto layer : layers) {
    delete layer;
  }
}

void ImageEditor::initUI() {}

void ImageEditor::renderUI() {
  // Painel lateral
  CV::color(0.2, 0.2, 0.2);
  CV::rectFill(canvasWidth - 200, 0, canvasWidth, canvasHeight);

  // Texto de status
  CV::color(1, 1, 1);
  CV::text(canvasWidth - 180, 30,
           ("Camada Atual: " + std::to_string(activeLayerIndex + 1)).c_str());
  CV::text(canvasWidth - 180, 60,
           ("Brush Size: " + std::to_string(brushSize)).c_str());
}

void ImageEditor::render() {
  CV::clear(0.8, 0.8, 0.8);

  renderCheckerBackground();
  renderLayers();
  renderUI();
}

void ImageEditor::renderCheckerBackground() {
  int checkerSize = 10;
  bool colorSwitch = true;

  for (int y = 0; y < canvasHeight; y += checkerSize) {
    for (int x = 0; x < canvasWidth - editorPanelWidth; x += checkerSize) {
      CV::color(colorSwitch ? 0.7 : 0.8, colorSwitch ? 0.7 : 0.8,
                colorSwitch ? 0.7 : 0.8);
      CV::rectFill(x, y, x + checkerSize, y + checkerSize);
      colorSwitch = !colorSwitch;
    }

    // Alterna a cor inicial a cada linha
    if ((canvasWidth / checkerSize) % 2 == 0)
      colorSwitch = !colorSwitch;
  }
}

void ImageEditor::handleMouse(int button, int state, int wheel, int direction,
                              int x, int y) {
  if (x > canvasWidth - editorPanelWidth)
    return;

  // Botão esquerdo pressionado
  if (button == 0 && state == 0) {
    drawing = true;
    if (activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
      layers[activeLayerIndex]->drawPixel(x, y, brushSize);
    }
  }

  // Botão esquerdo solto
  else if (button == 0 && state == 1) {
    drawing = false;
  }

  if (drawing && activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
    layers[activeLayerIndex]->drawPixel(x, y, brushSize);
  }
}

void ImageEditor::handleKeyboard(unsigned char key) {
  printf("\nTecla: %d", key);
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

void ImageEditor::handleKeyboardUp(unsigned char key) {
  printf("\nLiberou: %d", key);
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

void ImageEditor::addLayer() {
  // Cria uma nova camada com o tamanho do canvas
  Layer *newLayer = new Layer(canvasWidth, canvasHeight);
  // Adiciona a camada ao vetor de camadas
  layers.push_back(newLayer);
  // Define a camada ativa como a recém criada
  setActiveLayer(layers.size() - 1);
}

void ImageEditor::setActiveLayer(int index) {
  if (index >= 0 && index < layers.size()) {
    activeLayerIndex = index;
  }
}

bool ImageEditor::fileExists(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file) {
    fclose(file);
    return true; // Arquivo existe
  }
  printf("\nErro: Arquivo não encontrado - %s", filename);
  return false; // Arquivo não existe
}

void ImageEditor::loadImageToLayer(int layerIndex, const char *filename) {
  if (!fileExists(filename)) {
    return;
  }

  if (layerIndex >= 0 && layerIndex < layers.size()) {
    layers[layerIndex]->loadImage(filename);
  }
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
    if (activeLayerIndex == index)
      activeLayerIndex--;
  }
}

void ImageEditor::moveLayerDown(int index) {
  if (index >= 0 && index < layers.size() - 1) {
    std::swap(layers[index], layers[index + 1]);
    if (activeLayerIndex == index)
      activeLayerIndex++;
  }
}

void ImageEditor::toggleLayerVisibility(int index) {
  if (index >= 0 && index < layers.size()) {
    layers[index]->setVisible(!layers[index]->isVisible());
  }
}
