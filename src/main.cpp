/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario
conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         02/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>

#include "Bmp.h"
#include "ImageEditor.h"
#include "gl_canvas2d.h"

// Editor
ImageEditor *editor;

int screenWidth = 500, screenHeight = 500;
int mouseX, mouseY;

// BMP
Bmp *img1;
unsigned char *data;

void render() {
  editor->render();
  img1->render(img1, data);
}

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

  editor = new ImageEditor(screenWidth, screenHeight);

  img1 = new Bmp("./src/images/img1.bmp");
  data = img1->getImage();
  img1->convertBGRtoRGB();

  CV::run();
  return 0;
  delete editor;
}
