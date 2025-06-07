#include "LayerManager.h"

LayerManager::LayerManager(int width, int height)
{
    canvasWidth = width;
    canvasHeight = height;
    activeLayerIndex = 0;
    addNewLayer();
}

LayerManager::~LayerManager()
{
    for (auto layer : layers) {
        delete layer;
    }
}

void LayerManager::addNewLayer()
{
    Layer *newLayer = new Layer(canvasWidth, canvasHeight);
    layers.push_back(newLayer);
    setActiveLayer(layers.size() - 1);
}

void LayerManager::removeLayer(int index)
{
    validateIndex(index);

    if (layers.size() <= 1) {
        throw std::runtime_error("Cannot remove the last remaining layer");
    }

    layers.erase(layers.begin() + index);

    // Ajusta o índice ativo se necessário
    if (activeLayerIndex >= index) {
        activeLayerIndex = std::max(0, activeLayerIndex - 1);
    }
}

void LayerManager::setActiveLayer(int index)
{
    validateIndex(index);

    if (activeLayerIndex != index) {
        if (activeLayerIndex >= 0 && activeLayerIndex < layers.size()) {
            layers[activeLayerIndex]->setActive(false);
        }
        activeLayerIndex = index;
        layers[activeLayerIndex]->setActive(true);
    }
}

void LayerManager::toggleLayerVisibility(int index)
{
    validateIndex(index);
    layers[index]->setVisible(!layers[index]->isVisible());
}

void LayerManager::moveLayerUp(int index)
{
    validateIndex(index);

    if (index > 0) {
        std::swap(layers[index], layers[index - 1]);
        if (activeLayerIndex == index) {
            activeLayerIndex--;
        } else if (activeLayerIndex == index - 1) {
            activeLayerIndex++;
        }
    }
}

void LayerManager::moveLayerDown(int index)
{
    validateIndex(index);

    if (index < layers.size() - 1) {
        std::swap(layers[index], layers[index + 1]);
        if (activeLayerIndex == index) {
            activeLayerIndex++;
        } else if (activeLayerIndex == index + 1) {
            activeLayerIndex--;
        }
    }
}

void LayerManager::renderLayers(bool includeHidden) const
{
    for (const auto &layer : layers) {
        if (includeHidden || layer->isVisible()) {
            layer->render();
        }
    }
}

Layer *LayerManager::getLayer(int index)
{
    // Primeiro valida o índice
    validateIndex(index);

    // Retorna o raw pointer (ainda seguro porque o unique_ptr mantém a ownership)
    return layers[index];
}

const std::vector<Layer *> LayerManager::getAllLayers() const
{
    // Retorna uma referência constante para o vetor completo
    return layers;
}

Layer *LayerManager::getActiveLayer() { return layers[activeLayerIndex]; }

void LayerManager::validateIndex(int index) const
{
    if (index < 0 || index >= layers.size()) {
        throw std::out_of_range("Invalid layer index");
    }
}

size_t LayerManager::layerCount() const { return layers.size(); }

int LayerManager::getActiveLayerIndex() { return activeLayerIndex; }
