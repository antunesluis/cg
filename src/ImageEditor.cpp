#include "ImageEditor.h"

ImageEditor::ImageEditor(int width, int height, int panelWidth) {
  canvasWidth = width;
  canvasHeight = height;
  editorPanelWidth = panelWidth;
  drawing = false;
  brushSize = config::brush::default_size;

  // Cor inicial do pincel (vermelho)
  currentColor[0] = 255;
  currentColor[1] = 0;
  currentColor[2] = 0;

  uiManager = new UIManager(panelWidth);
  layerManager = new LayerManager(width, height);
  initUI();
}

ImageEditor::~ImageEditor() {
  delete layerManager;
  delete uiManager;
}

void ImageEditor::initUI() {
  float buttonWidth = editorPanelWidth - 2 * config::layout::side_margin;
  float buttonHeight = config::layout::button_height;
  float startY = config::layout::button_start_y;
  float margin = config::layout::button_margin;

  uiManager->addButton(config::layout::side_margin, startY, buttonWidth,
                       buttonHeight, "Adicionar Layer",
                       [this]() { this->layerManager->addNewLayer(); });

  uiManager->addButton(config::layout::side_margin,
                       startY + buttonHeight + margin, buttonWidth,
                       buttonHeight, "Remover Layer", [this]() {
                         if (layerManager->layerCount() > 1) {
                           this->layerManager->removeLayer(
                               this->layerManager->getActiveLayerIndex());
                         }
                       });

  uiManager->addButton(config::layout::side_margin,
                       startY + 2 * (buttonHeight + margin), buttonWidth,
                       buttonHeight, "Carregar Imagem", [this]() {
                         this->imageLoaderUI.setVisible(
                             true, this->layerManager->getActiveLayer());
                       });
}

void ImageEditor::handleMouse(int button, int state, int wheel, int direction,
                              int x, int y) {
  if (placingImage) {
    previewX = x;
    previewY = y;
  }

  // Verifica seleção no imageLoaderUI
  if (imageLoaderUI.handleMouse(button, state, x, y)) {
    placingImage = true;
    return;
  }

  // Posiciona a imagem ao clicar
  if (placingImage && button == 0 && state == 0) {
    Layer *activeLayer = layerManager->getActiveLayer();
    if (activeLayer && activeLayer->hasImage()) {
      // Centraliza a imagem no clique
      Bmp *img = activeLayer->getImage();
      activeLayer->setPosition(x - img->getWidth() / 2,
                               y - img->getHeight() / 2);
    }
    placingImage = false;
    return;
  }

  if (x > canvasWidth - editorPanelWidth) {
    x -= (canvasWidth - editorPanelWidth);

    handleLayerListClick(x, y, button, state);
    uiManager->handleMouse(x, y, state);
    return;
  }

  if (button == 0) {
    if (state == 0) {
      drawing = true;
      Layer *activeLayer = layerManager->getActiveLayer();
      if (activeLayer) {
        activeLayer->setDrawingColor(currentColor[0], currentColor[1],
                                     currentColor[2]);
        activeLayer->drawPixel(x, y, brushSize);
      }
    } else {
      drawing = false;
    }
  }

  if (drawing) {
    Layer *activeLayer = layerManager->getActiveLayer();
    if (activeLayer) {
      activeLayer->drawPixel(x, y, brushSize);
    }
  }
}

void ImageEditor::handleKeyboard(unsigned char key) {
  switch (key) {
  case 27:
    exit(0);
  case '1':
    currentColor[0] = 255;
    currentColor[1] = 0;
    currentColor[2] = 0;
    break;
  case '2':
    currentColor[0] = 0;
    currentColor[1] = 255;
    currentColor[2] = 0;
    break;
  case '3':
    currentColor[0] = 0;
    currentColor[1] = 0;
    currentColor[2] = 255;
    break;
  case '+':
    brushSize = std::min(brushSize + 1, config::brush::max_size);
    break;
  case '-':
    brushSize = std::max(brushSize - 1, config::brush::min_size);
    break;
  }
}

void ImageEditor::handleKeyboardUp(unsigned char key) {
  // (vazio por enquanto)
}

void ImageEditor::handleLayerListClick(int x, int y, int button, int state) {
  float listStartY = config::layout::layer_list_start_y;
  float itemHeight = config::layout::layer_item_height;
  float listEndY = listStartY + layerManager->layerCount() * itemHeight;

  // Verifica se o cliqe foi dentro da área da lista de camadas
  if (y >= listStartY && y <= listEndY && x >= 5 && x <= editorPanelWidth - 5) {
    if (button == 0 && state == 0) {
      int clickedLayer = static_cast<int>((y - listStartY) / itemHeight);

      // Verifica se o clique foi dentro da checkbox
      if (x >= editorPanelWidth - 30 && x <= editorPanelWidth - 10) {
        layerManager->toggleLayerVisibility(clickedLayer);
      } else if (x <= editorPanelWidth - 40) {
        layerManager->setActiveLayer(clickedLayer);
      }
    }
  }
}

void ImageEditor::handleImagePlacement(int x, int y) {
  Layer *activeLayer = layerManager->getActiveLayer();
  if (activeLayer && activeLayer->hasImage()) {
    Bmp *img = activeLayer->getImage();

    // Calcula posição centralizada
    float drawX = previewX - img->getWidth() / 2;
    float drawY = previewY - img->getHeight() / 2;

    // Usa drawImage com offsets explícitos
    activeLayer->renderImage(drawX, drawY);

    // Desenha retângulo de seleção
    CV::color(1, 1, 0);
    CV::rect(drawX, drawY, drawX + img->getWidth(), drawY + img->getHeight());
  }

  // Texto de instrução
  CV::color(1, 1, 1);
  CV::text(10, 30, "Clique para posicionar a imagem");
}

void ImageEditor::render(int mouseX, int mouseY) {
  CV::clear(0.8f, 0.8f, 0.8f);

  renderCheckerBackground();
  renderPanelBackground();

  // Renderiza as camadas primeiro
  layerManager->renderLayers();

  // Renderiza o preview da imagem sendo posicionada
  if (placingImage) {
    handleImagePlacement(mouseX, mouseY);
  }
  imageLoaderUI.render(mouseX, mouseY);

  renderUI();
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

void ImageEditor::renderUI() {
  CV::translate(canvasWidth - editorPanelWidth, 0);
  uiManager->render();
  renderLayersList();
  renderBrushInfo();
  CV::translate(-(canvasWidth - editorPanelWidth), 0);
}

void ImageEditor::renderPanelBackground() {
  CV::color(config::colors::panel_bg[0], config::colors::panel_bg[1],
            config::colors::panel_bg[2]);
  CV::rectFill(canvasWidth - editorPanelWidth, 0, canvasWidth, canvasHeight);
}

void ImageEditor::renderLayersList() {
  float listStartY = config::layout::layer_list_start_y;
  float itemHeight = config::layout::layer_item_height;
  int activeLayerIndex = layerManager->getActiveLayerIndex();

  CV::color(config::colors::text[0], config::colors::text[1],
            config::colors::text[2]);
  CV::text(10, listStartY - 20, "Camadas:");

  for (int i = 0; i < layerManager->layerCount(); i++) {
    float y = listStartY + i * itemHeight;

    // Destaque para layer ativa
    if (i == activeLayerIndex) {
      CV::color(config::colors::active_layer[0],
                config::colors::active_layer[1],
                config::colors::active_layer[2]);
      CV::rectFill(5, y - 10, editorPanelWidth - 5, y + itemHeight - 5);
    }

    // Nome da layer
    CV::color(config::colors::text[0], config::colors::text[1],
              config::colors::text[2]);
    CV::text(10, y + 10, ("Layer " + std::to_string(i + 1)).c_str());

    // Cria e renderiza o checkbox (pode ser otimizado para criar apenas uma
    // vez)
    Layer *layer = layerManager->getLayer(i);
    Checkbox cb(editorPanelWidth - 30, y, config::layout::checkbox_size, "",
                layer->isVisible());
    cb.render();
  }
}

void ImageEditor::renderBrushInfo() {
  CV::color(config::colors::text[0], config::colors::text[1],
            config::colors::text[2]);
  CV::text(10, canvasHeight - 60,
           ("Tamanho: " + std::to_string(brushSize)).c_str());

  CV::color(currentColor[0] / 255.0f, currentColor[1] / 255.0f,
            currentColor[2] / 255.0f);
  CV::rectFill(10, canvasHeight - 40, 30, canvasHeight - 20);
  CV::color(0, 0, 0);
  CV::rect(10, canvasHeight - 40, 30, canvasHeight - 20);
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
  if (!fileExists(filename) || layerIndex < 0 ||
      layerIndex >= layerManager->layerCount()) {
    return;
  }
  Layer *layer = layerManager->getLayer(layerIndex);
  layer->loadImage(filename);
}
