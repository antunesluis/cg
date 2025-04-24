/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
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
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int opcao = 50;     // variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; // variaveis globais do mouse para poder exibir dentro da render().

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10, 300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10, 320, str);
}

// funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
// Todos os comandos para desenho na canvas devem ser chamados dentro da render().
// Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::clear(1, 1, 1);

    CV::color(0.5, 0.5, 0.5);
    CV::text(20, 500, "Programa Demo Canvas2D");
    DrawMouseScreenCoords();

    // Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    printf("\nTecla: %d", key);
    if (key < 200) {
        opcao = key;
    }

    switch (key) {
    case 27:
        exit(0);
        break;
    }
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key) { printf("\nLiberou: %d", key); }

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x; // guarda as coordenadas do mouse para exibir dentro da render()
    mouseY = y;

    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
    CV::run();
}
