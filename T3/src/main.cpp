#include "GameManager.h"
#include "gl_canvas2d.h"
#include <GL/glut.h>
#include <ctime>
#include <math.h>
#include <stdlib.h>

// Largura e altura da tela
int screenWidth = 1920, screenHeight = 1080;

// Vari�veis globais do mouse
int mouseX, mouseY;

GameManager *gameManager = nullptr;

// Renderiza��o conforme o estado do jogo
void render() {
  if (gameManager != nullptr) {
    gameManager->update();
    gameManager->render();
  }
}

// Fun��o chamada quando o mouse � usado
void mouse(int button, int state, int wheel, int direction, int x, int y) {
  if (gameManager != nullptr) {
    gameManager->mouse(button, state, wheel, direction, x, y);
  }
}

// Fun��o chamada quando uma tecla � pressionada
void keyboard(int key) {
  if (gameManager != nullptr) {
    gameManager->keyboard(key);
  }
}

// Fun��o chamada quando uma tecla � liberada
void keyboardUp(int key) {
  if (gameManager != nullptr) {
    gameManager->keyboardUp(key);
  }
}

int main(void) {
  srand(time(NULL));

  // Initialize Canvas
  CV::init(&screenWidth, &screenHeight, "Trabalho 3 - Tanque 2D");

  // Initialize Game Manager
  gameManager = new GameManager();

  // Run main loop
  CV::run();

  // Cleanup
  delete gameManager;
  gameManager = nullptr;

  return 0;
}
