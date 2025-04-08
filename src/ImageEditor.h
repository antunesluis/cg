#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "Checkbox.h"
#include "Config.h"
#include "ImageLoaderUI.h"
#include "Layer.h"
#include "LayerManager.h"
#include "UIManager.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <string>
#include <vector>

class ImageEditor {
private:
  UIManager *uiManager;
  LayerManager *layerManager;

  ImageLoaderUI imageLoaderUI;
  bool placingImage = false;
  float placementX, placementY;
  std::string currentImagePath;
  float previewX, previewY;

  bool drawing;
  int brushSize;
  unsigned char currentColor[3];
  int canvasWidth, canvasHeight, editorPanelWidth;
  int scrollOffset = 0;

public:
  ImageEditor(int width, int height, int panelWidth);
  ~ImageEditor();

  // Canva handling methods
  void render(int mouseX, int mouseY);
  void handleMouse(int button, int state, int wheel, int direction, int x,
                   int y);
  void handleKeyboard(unsigned char key);
  void handleKeyboardUp(unsigned char key);

  // Image operations
  void loadImageToLayer(int layerIndex, const char *filename);
  void handleImagePlacement(int x, int y);

private:
  void renderCheckerBackground();
  void renderPanelBackground();
  void renderLayersList();
  void renderLayers();
  void renderBrushInfo();
  void renderUI();

  void initUI();
  bool fileExists(const char *filename);
  void handleLayerListClick(int x, int y, int button, int state);
};

#endif
