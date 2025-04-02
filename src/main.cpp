#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>

#include "ImageEditor.h"
#include "gl_canvas2d.h"

// Editor
ImageEditor *editor;

int screenWidth = 1000, screenHeight = 1000, editorPanelWidth = 200;
int mouseX, mouseY;

void render() { editor->render(); }

void keyboard(int key) { editor->handleKeyboard(key); }

void keyboardUp(int key) { editor->handleKeyboardUp(key); }

void mouse(int button, int state, int wheel, int direction, int x, int y) {
  mouseX = x;
  mouseY = y;

  editor->handleMouse(button, state, wheel, direction, x, y);
}

int main(void) {
  CV::init(&screenWidth, &screenHeight,
           "Photoshop Caseiro - Computacao Grafica");

  editor = new ImageEditor(screenWidth, screenHeight, editorPanelWidth);

  // Carrega a imagem apenas através do editor
  editor->loadImageToLayer(0, "./src/images/img1.bmp");

  CV::run();

  delete editor;
  return 0;
}
