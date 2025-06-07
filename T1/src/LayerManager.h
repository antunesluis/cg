#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "Layer.h"
#include <stdexcept>
#include <vector>

class LayerManager
{
  private:
    std::vector<Layer *> layers;
    int activeLayerIndex;
    int canvasWidth;
    int canvasHeight;

  public:
    LayerManager(int canvasWidth, int canvasHeight);
    ~LayerManager();

    // Gerenciamento básico de layers
    void addNewLayer();
    void removeLayer(int index);

    // Controle de ordem e visibilidade
    void moveLayerUp(int index);
    void moveLayerDown(int index);
    void toggleLayerVisibility(int index);
    void setLayerVisibility(int index, bool visible);
    size_t layerCount() const;

    // Controle de layer ativa
    void setActiveLayer(int index);
    int getActiveLayerIndex();

    // Acesso às layers
    Layer *getLayer(int index);
    const std::vector<Layer *> getAllLayers() const;
    Layer *getActiveLayer();

    // Controle de renderização
    void renderLayers(bool includeHidden = false) const;

  private:
    void validateIndex(int index) const;
};

#endif // LAYERMANAGER_H
