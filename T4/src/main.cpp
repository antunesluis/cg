#include "gl_canvas2d.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int screenWidth = 1280, screenHeight = 720;

void render()
{
    CV::clear(0, 0, 0);

    CV::translate(screenWidth / 2.0, screenHeight / 2.0);
    CV::color(0.5, 0.5, 0.5);
    CV::line(0, -screenHeight / 2.0, 0, screenHeight / 2.0);
}

void keyboard(int key) { printf("\nTecla: %d", key); }

void keyboardUp(int key) { printf("\nLiberou: %d", key); }

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Canva 2D - Demo"), CV::run();

    return 0;
}
