#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

// #include "UIManager.h"
#include "Checkbox.h"
#include "Layer.h"
#include "UIManager.h"
#include <vector>

class ImageEditor {
private:
  UIManager *uiManager;
  std::vector<Layer *> layers;
  std::vector<Checkbox *> layerVisibilityCheckboxes;

  int activeLayerIndex;
  bool drawing;
  int brushSize;
  unsigned char currentColor[3];
  int canvasWidth, canvasHeight, editorPanelWidth;
  int scrollOffset = 0;

public:
  ImageEditor(int width, int height, int panelWidth);
  ~ImageEditor();

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
  void moveLayerUp(int index);
  void moveLayerDown(int index);
  void toggleLayerVisibility(int index);

  // Image operations
  void loadImageToLayer(int layerIndex, const char *filename);
  void applyEffectToActiveLayer(int effectType, float param = 0.0f);

private:
  void renderCheckerBackground();
  void renderLayers();
  void renderUI();
  void initUI();
  bool fileExists(const char *filename);
};

#endif
