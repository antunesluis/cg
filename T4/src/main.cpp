#include "Constants.h"
#include "Editor.h"
#include "gl_canvas2d.h"

Editor *editor = nullptr;
int screenWidth = Constants::SCREEN_WIDTH;
int screenHeight = Constants::SCREEN_HEIGHT;

void render()
{
    if (editor) {
        editor->render();
        editor->update();
    }
}

void keyboard(int key)
{
    if (editor) {
        editor->handleKeyboard(key);
    }
}

void keyboardUp(int key)
{
    if (editor) {
        editor->handleKeyboardUp(key);
    }
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    if (editor) {
        editor->onMouse(button, state, wheel, direction, x, y);
    }
}

int main(void)
{
    editor = new Editor(screenWidth, screenHeight);
    editor->initialize();

    CV::init(&screenWidth, &screenHeight, "Trabalho 4");
    CV::run();

    delete editor;
    editor = nullptr;

    return 0;
}
