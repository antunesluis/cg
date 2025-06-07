#include "ImageEditor.h"
#include "Config.h"

ImageEditor::ImageEditor(int width, int height, int panelWidth)
{
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

ImageEditor::~ImageEditor()
{
    delete layerManager;
    delete uiManager;
}

void ImageEditor::initUI()
{
    float buttonWidth = editorPanelWidth - 2 * config::layout::side_margin;
    float buttonHeight = config::layout::button_height;
    float margin = config::layout::button_margin;
    float currentY = config::layout::button_start_y;

    // ===== SEÇÃO DE GERENCIAMENTO DE CAMADAS =====
    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Adicionar Camada",
                         [this]() { this->layerManager->addNewLayer(); });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Remover Camada", [this]() {
        if (layerManager->layerCount() > 1) {
            this->layerManager->removeLayer(this->layerManager->getActiveLayerIndex());
        }
    });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Mover p/ Cima",
                         [this]() { this->layerManager->moveLayerUp(this->layerManager->getActiveLayerIndex()); });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Mover p/ Baixo",
                         [this]() { this->layerManager->moveLayerDown(this->layerManager->getActiveLayerIndex()); });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Carregar Imagem",
                         [this]() { this->imageLoaderUI.setVisible(true, this->layerManager->getActiveLayer()); });
    currentY += buttonHeight + margin;

    // ===== LISTA DE LAYERS =====
    // (Renderizada automaticamente na posição layer_list_start_y)

    // ===== SEÇÃO DE FERRAMENTAS =====
    currentY = config::layout::effects_start_y;
    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Pincel",
                         [this]() { this->currentTool = ToolType::BRUSH; });
    currentY += buttonHeight + margin;

    // ===== SEÇÃO DE EFEITOS =====
    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Flip Horizontal", [this]() {
        Layer *layer = layerManager->getActiveLayer();
        if (layer)
            layer->flipHorizontal();
    });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Flip Vertical", [this]() {
        Layer *layer = layerManager->getActiveLayer();
        if (layer)
            layer->flipVertical();
    });
    currentY += buttonHeight + margin;

    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Tons de Cinza", [this]() {
        Layer *layer = layerManager->getActiveLayer();
        if (layer)
            layer->convertToGrayscale();
    });
    currentY += buttonHeight + margin;

    // ===== SLIDERS DE AJUSTE =====
    uiManager->addSlider(config::layout::side_margin, currentY, buttonWidth, 30, 0.5f, 1.5f, 1.0f, "Brilho",
                         [this](float value) {
                             Layer *layer = layerManager->getActiveLayer();
                             if (layer) {
                                 this->currentBrightness = value;
                                 this->updateImageEffects();
                             }
                         });
    currentY += 40;

    uiManager->addSlider(config::layout::side_margin, currentY, buttonWidth, 30, 0.0f, 2.0f, 1.0f, "Contraste",
                         [this](float value) {
                             Layer *layer = layerManager->getActiveLayer();
                             if (layer) {
                                 this->currentContrast = value;
                                 this->updateImageEffects();
                             }
                         });
    currentY += 40;

    uiManager->addSlider(config::layout::side_margin, currentY, buttonWidth, 30, 0.0f, 2.0f, 1.0f, "Saturacao",
                         [this](float value) {
                             Layer *layer = layerManager->getActiveLayer();
                             if (layer) {
                                 this->currentSaturation = value;
                                 this->updateImageEffects();
                             }
                         });
    currentY += 40;

    uiManager->addSlider(config::layout::side_margin, currentY, buttonWidth, 30, config::brush::min_size,
                         config::brush::max_size, config::brush::default_size, "Brush Size",
                         [this](float value) { this->brushSize = static_cast<int>(value); });
    currentY += 50;

    // ===== UTILITÁRIOS =====
    uiManager->addButton(config::layout::side_margin, currentY, buttonWidth, buttonHeight, "Limpar Camada", [this]() {
        Layer *layer = layerManager->getActiveLayer();
        if (layer) {
            layer->clear();
            layer->resetEffects();
        }
    });
}

void ImageEditor::updateImageEffects()
{
    Layer *activeLayer = layerManager->getActiveLayer();
    if (activeLayer && activeLayer->hasImage()) {
        activeLayer->applyEffects(currentBrightness, currentContrast, currentSaturation);
    }
}

void ImageEditor::handleMouse(int button, int state, int wheel, int direction, int x, int y)
{
    if (button == 0 && state == 0) { // Botão esquerdo pressionado
        if (handleColorPickerClick(x, y)) {
            return; // Se selecionou uma cor, não faz outras ações
        }
    }

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
            // Centraliza a imagem no clique usando as dimensões da layer
            activeLayer->setPosition(x - activeLayer->getImageWidth() / 2, y - activeLayer->getImageHeight() / 2);
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
                activeLayer->setDrawingColor(currentColor[0], currentColor[1], currentColor[2]);
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

void ImageEditor::handleKeyboard(unsigned char key)
{
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

void ImageEditor::handleKeyboardUp(unsigned char key)
{
    // (vazio por enquanto)
}

void ImageEditor::handleLayerListClick(int x, int y, int button, int state)
{
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

void ImageEditor::handleImagePlacement(int x, int y)
{
    Layer *activeLayer = layerManager->getActiveLayer();
    if (activeLayer && activeLayer->hasImage()) {
        // Calcula posição centralizada
        float drawX = previewX - activeLayer->getImageWidth() / 2;
        float drawY = previewY - activeLayer->getImageHeight() / 2;

        // Renderiza a imagem
        activeLayer->renderImage(drawX, drawY);

        // Desenha retângulo de seleção
        CV::color(1, 1, 0);
        CV::rect(drawX, drawY, drawX + activeLayer->getImageWidth(), drawY + activeLayer->getImageHeight());
    }

    // Texto de instrução
    CV::color(1, 1, 1);
    CV::text(10, 30, "Clique para posicionar a imagem");
}

void ImageEditor::render(int mouseX, int mouseY)
{
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

    renderColorPicker();
    renderUI();
}

void ImageEditor::renderCheckerBackground()
{
    int checkerSize = 10;
    bool colorSwitch = true;

    for (int y = 0; y < canvasHeight; y += checkerSize) {
        for (int x = 0; x < canvasWidth - editorPanelWidth; x += checkerSize) {
            CV::color(colorSwitch ? 0.7f : 0.8f, colorSwitch ? 0.7f : 0.8f, colorSwitch ? 0.7f : 0.8f);
            CV::rectFill(x, y, x + checkerSize, y + checkerSize);
            colorSwitch = !colorSwitch;
        }

        if (((canvasWidth - editorPanelWidth) / checkerSize) % 2 == 0) {
            colorSwitch = !colorSwitch;
        }
    }
}

void ImageEditor::renderUI()
{
    CV::translate(canvasWidth - editorPanelWidth, 0);

    // Renderiza os elementos UI
    uiManager->render();

    // Linha separadora acima da lista de layers
    CV::color(0.4f, 0.4f, 0.5f);
    CV::line(10, config::layout::layer_list_start_y - 35, editorPanelWidth - 10,
             config::layout::layer_list_start_y - 35);

    // Linha separadora acima das ferramentas
    CV::line(10, config::layout::effects_start_y - 15, editorPanelWidth - 10, config::layout::effects_start_y - 15);

    renderLayersList();
    renderBrushInfo();
    CV::translate(-(canvasWidth - editorPanelWidth), 0);
}

void ImageEditor::renderPanelBackground()
{
    CV::color(config::colors::panel_bg[0], config::colors::panel_bg[1], config::colors::panel_bg[2]);
    CV::rectFill(canvasWidth - editorPanelWidth, 0, canvasWidth, canvasHeight);
}

void ImageEditor::renderLayersList()
{
    float listStartY = config::layout::layer_list_start_y;
    float itemHeight = config::layout::layer_item_height;
    int activeLayerIndex = layerManager->getActiveLayerIndex();

    // Adiciona um fundo e título para a seção
    CV::color(config::colors::panel_bg[0], config::colors::panel_bg[1], config::colors::panel_bg[2]);
    CV::rectFill(0, listStartY - 30, editorPanelWidth, listStartY + layerManager->layerCount() * itemHeight + 10);

    CV::color(config::colors::text[0], config::colors::text[1], config::colors::text[2]);
    CV::text(10, listStartY - 15, "LISTA DE CAMADAS:");

    for (int i = 0; i < layerManager->layerCount(); i++) {
        float y = listStartY + i * itemHeight;

        // Destaque para layer ativa
        if (i == activeLayerIndex) {
            CV::color(config::colors::active_layer[0], config::colors::active_layer[1],
                      config::colors::active_layer[2]);
            CV::rectFill(5, y - 10, editorPanelWidth - 5, y + itemHeight - 5);
        }

        // Nome da layer
        CV::color(config::colors::text[0], config::colors::text[1], config::colors::text[2]);
        CV::text(10, y + 10, ("Layer " + std::to_string(i + 1)).c_str());

        // Cria e renderiza o checkbox (pode ser otimizado para criar apenas uma
        // vez)
        Layer *layer = layerManager->getLayer(i);
        Checkbox cb(editorPanelWidth - 30, y, config::layout::checkbox_size, "", layer->isVisible());
        cb.render();
    }
}

void ImageEditor::renderBrushInfo()
{
    CV::color(config::colors::text[0], config::colors::text[1], config::colors::text[2]);
    CV::text(10, canvasHeight - 60, ("Tamanho: " + std::to_string(brushSize)).c_str());

    CV::color(currentColor[0] / 255.0f, currentColor[1] / 255.0f, currentColor[2] / 255.0f);
    CV::rectFill(10, canvasHeight - 40, 30, canvasHeight - 20);
    CV::color(0, 0, 0);
    CV::rect(10, canvasHeight - 40, 30, canvasHeight - 20);
}

bool ImageEditor::fileExists(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void ImageEditor::renderColorPicker()
{
    float startX = canvasWidth - editorPanelWidth + config::layout::side_margin;
    float startY = config::layout::color_picker_start_y;
    float boxSize = config::layout::color_box_size;
    float margin = config::layout::color_box_margin;
    int colorsPerRow = 8;

    // Título
    CV::color(config::colors::text[0], config::colors::text[1], config::colors::text[2]);
    CV::text(startX, startY - 20, "Seletor de Cores:");

    // Renderiza os quadrados de cor
    for (size_t i = 0; i < config::colors::colorPickerColors.size(); i++) {
        int row = i / colorsPerRow;
        int col = i % colorsPerRow;

        float x = startX + col * (boxSize + margin);
        float y = startY + row * (boxSize + margin);

        // Cor do quadrado
        CV::color(config::colors::colorPickerColors[i][0], config::colors::colorPickerColors[i][1],
                  config::colors::colorPickerColors[i][2]);
        CV::rectFill(x, y, x + boxSize, y + boxSize);

        // Borda (mais escura quando selecionada)
        bool isCurrentColor = (currentColor[0] == config::colors::colorPickerColors[i][0] * 255 &&
                               currentColor[1] == config::colors::colorPickerColors[i][1] * 255 &&
                               currentColor[2] == config::colors::colorPickerColors[i][2] * 255);

        CV::color(isCurrentColor ? 0.0f : 0.3f, isCurrentColor ? 0.0f : 0.3f, isCurrentColor ? 0.0f : 0.3f);
        CV::rect(x, y, x + boxSize, y + boxSize);
    }
}

bool ImageEditor::handleColorPickerClick(int x, int y)
{
    float startX = canvasWidth - editorPanelWidth + config::layout::side_margin;
    float startY = config::layout::color_picker_start_y;
    float boxSize = config::layout::color_box_size;
    float margin = config::layout::color_box_margin;
    int colorsPerRow = 8;
    int totalRows = 2;

    // Verifica se o clique foi dentro da área do color picker
    if (x < startX || x > startX + colorsPerRow * (boxSize + margin) || y < startY ||
        y > startY + totalRows * (boxSize + margin)) {
        return false;
    }

    // Calcula qual cor foi clicada
    int col = static_cast<int>((x - startX) / (boxSize + margin));
    int row = static_cast<int>((y - startY) / (boxSize + margin));
    int index = row * colorsPerRow + col;

    if (index >= 0 && index < static_cast<int>(config::colors::colorPickerColors.size())) {
        // Atualiza a cor atual (convertendo de float [0-1] para unsigned char
        // [0-255])
        currentColor[0] = static_cast<unsigned char>(config::colors::colorPickerColors[index][0] * 255);
        currentColor[1] = static_cast<unsigned char>(config::colors::colorPickerColors[index][1] * 255);
        currentColor[2] = static_cast<unsigned char>(config::colors::colorPickerColors[index][2] * 255);
        return true;
    }

    return false;
}

void ImageEditor::loadImageToLayer(int layerIndex, const char *filename)
{
    if (!fileExists(filename) || layerIndex < 0 || layerIndex >= layerManager->layerCount()) {
        return;
    }
    Layer *layer = layerManager->getLayer(layerIndex);
    layer->loadImage(filename);
}
