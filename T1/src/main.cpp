/************************************************************************************************
// Luis Fernando Antunes - Editor de imagens Simplificado
// Trabalho 1 de Computação Gáfica - Desenvolvido usando a API Gráfica Canvas2D
// LEIA O README.md DO PROJETO PARA INFORMAÇÔES IMPORTANTES SOBRE O FUNCIONAMENTO E IMPLEMENTAÇÂO
// **********************************************************************************************/

#include <GL/glut.h>

#include <dirent.h>
#include <math.h>
#include <stdlib.h>

#include "Config.h"
#include "ImageEditor.h"
#include "gl_canvas2d.h"

// Editor
ImageEditor *editor;
int mouseX, mouseY;

void render() { editor->render(mouseX, mouseY); }

void keyboard(int key) { editor->handleKeyboard(key); }

void keyboardUp(int key) { editor->handleKeyboardUp(key); }

void mouse(int button, int state, int wheel, int direction, int x, int y) {
  mouseX = x;
  mouseY = y;

  editor->handleMouse(button, state, wheel, direction, x, y);
}

int main(void) {
  int screenHeight = config::screen::height;
  int screenWidth = config::screen::width;
  int editorPanelWidth = config::editor::panel_width;

  CV::init(&screenWidth, &screenHeight,
           "Photoshop Caseiro - Computacao Grafica");

  editor = new ImageEditor(screenWidth, screenHeight, editorPanelWidth);
  printf("UI Manager criado. Painel width: %d\n", editorPanelWidth);

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir("./src/images/")) != NULL) {
    printf("\nArquivos encontrados na pasta images:\n");
    while ((ent = readdir(dir)) != NULL) {
      std::string name = ent->d_name;
      if (name.size() > 4 && name.substr(name.size() - 4) == ".bmp") {
        printf("- %s\n", name.c_str());
      }
    }
    closedir(dir);
  } else {
    printf("\nERRO: Nao foi possivel abrir a pasta ./src/images/\n");
  }

  CV::run();

  delete editor;
  return 0;
}
