#include "GameManager.h"
#include "gl_canvas2d.h"
#include <GL/glut.h>
#include <ctime>
#include <math.h>
#include <stdlib.h>

// Largura e altura da tela
int screenWidth = 1920, screenHeight = 1080;

// Variáveis globais do mouse
int mouseX, mouseY;

GameManager *gameManager = nullptr;

// Renderização conforme o estado do jogo
void render() {
  if (gameManager != nullptr) {
    gameManager->update();
    gameManager->render();
  }
}

// Função chamada quando o mouse é usado
void mouse(int button, int state, int wheel, int direction, int x, int y) {
  if (gameManager != nullptr) {
    gameManager->mouse(button, state, wheel, direction, x, y);
  }
}

// Função chamada quando uma tecla é pressionada
void keyboard(int key) {
  if (gameManager != nullptr) {
    gameManager->keyboard(key);
  }
}

// Função chamada quando uma tecla é liberada
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
