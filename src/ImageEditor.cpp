#include "ImageEditor.h"
#include "gl_canvas2d.h"

ImageEditor::ImageEditor(int w, int h) {
  canvasWidth = w;
  canvasHeight = h;
  activeLayerIndex = 0;
  drawing = false;
  brushSize = 3;

  // Cor inicial: vermelho
  currentColor[0] = 255;
  currentColor[1] = 0;
  currentColor[2] = 0;
}

ImageEditor::~ImageEditor() {}

void ImageEditor::initUI() {}

void ImageEditor::render() {
  CV::clear(0.8, 0.8, 0.8);

  // Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

void ImageEditor::handleMouse(int button, int state, int wheel, int direction,
                              int x, int y) {
  printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);
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
