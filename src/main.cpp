#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>

#include "Constants.h"
#include "ImageEditor.h"
#include "gl_canvas2d.h"

// Editor
ImageEditor *editor;
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
  int screenHeight = Constants::SCREEN_HEIGHT;
  int screenWidth = Constants::SCREEN_WIDTH;
  int editorPanelWidth = Constants::EDITOR_PANEL_WIDTH;

  CV::init(&screenWidth, &screenHeight,
           "Photoshop Caseiro - Computacao Grafica");

  editor = new ImageEditor(screenWidth, screenHeight, editorPanelWidth);
  printf("UI Manager criado. Painel width: %d\n", editorPanelWidth);

  // Carrega a imagem apenas através do editor
  editor->loadImageToLayer(0, "./src/images/img1.bmp");
  // editor->loadImageToLayer(0, ".\\src\\images\\img1.bmp");

  CV::run();

  delete editor;
  return 0;
}
