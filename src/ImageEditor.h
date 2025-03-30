#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

// #include "Layer.h"
// #include "UIManager.h"
#include <vector>

class ImageEditor {
private:
  // std::vector<Layer *> layers;
  // UIManager *uiManager;
  int activeLayerIndex;
  bool drawing;
  int brushSize;
  unsigned char currentColor[3];
  int canvasWidth, canvasHeight;

public:
  ImageEditor(int w, int h);
  ~ImageEditor();
  void initUI();

  // Canva handling methods
  void render();
  void handleMouse(int button, int state, int wheel, int direction, int x,
                   int y);
  void handleKeyboard(unsigned char key);
  void handleKeyboardUp(unsigned char key);

  // Layer management
  void addLayer();
  void removeLayer(int index);
  void setActiveLayer(int index);
  // void moveLayerUp(int index);
  // void moveLayerDown(int index);
  // void toggleLayerVisibility(int index);

  // Image operations
  void loadImageToLayer(int layerIndex, const char *filename);
  bool fileExists(const char *filename);
  // void applyEffectToActiveLayer(int effectType, float param = 0.0f);

private:
  // void renderCheckerBackground();
  void renderLayers();
  // void renderUI();
};

#endif
