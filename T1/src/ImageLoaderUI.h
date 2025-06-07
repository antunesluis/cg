// ImageLoaderUI.h
#ifndef IMAGE_LOADER_UI_H
#define IMAGE_LOADER_UI_H

#include "Bmp.h"
#include "Layer.h"
#include <string>
#include <vector>

class ImageLoaderUI
{
  private:
    std::vector<std::string> imagePaths;
    bool visible = false;
    bool imageSelected = false;

    int mouseX = 0, mouseY = 0;
    float posX, posY;    // Posição do modal
    float width, height; // Tamanho do modal
    bool dragging = false;
    float dragOffsetX, dragOffsetY;

    Layer *targetLayer;

  public:
    ImageLoaderUI();
    void setVisible(bool visible, Layer *targetLayer = nullptr);
    void render(int mouseX, int mouseY);
    bool handleMouse(int button, int state, int x, int y);
    void updateImageList();
};

#endif
