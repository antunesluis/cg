#include "Editor.h"
#include "gl_canvas2d.h"
#include <stdio.h>

Editor::Editor(int width, int height) : isDragging(false), selectedPoint(-1)
{
    screenWidth = width;
    screenHeight = height;
}

void Editor::initialize() { initializeCurve(); }

void Editor::initializeCurve()
{
    bezierCurve = BCurve2D();
    selectedPoint = -1;
    isDragging = false;
}

void Editor::render()
{
    CV::clear(0, 0, 0);

    if (!bezierCurve.isEmpty()) {
        bezierCurve.drawControlPoints(selectedPoint);
        bezierCurve.drawCurve();
    }

    drawInfo();
}

void Editor::update()
{
    //
}

void Editor::drawInfo()
{
    char info[300];
    sprintf(info, "Pontos de Controle: %zu | Grau da Curva: %zu | Pontos da Curva: %zu",
            bezierCurve.getControlPointCount(),
            bezierCurve.getControlPointCount() > 0 ? bezierCurve.getControlPointCount() - 1 : 0,
            bezierCurve.getCurvePointCount());

    CV::color(1, 1, 1); // Branco para o texto
    CV::text(10, screenHeight - 30, info);
}

void Editor::onMouse(int button, int state, int wheel, int direction, int x, int y)
{
    printf("\nMouse: button=%d state=%d wheel=%d direction=%d x=%d y=%d", button, state, wheel, direction, x, y);

    if (button == -2 && state == -2) { // Movimento do mouse
        handleMouseMotion(x, y);
    } else {
        handleMouseClick(button, state, x, y);
    }
}

void Editor::handleMouseClick(int button, int state, int x, int y)
{
    if (button == 0) {    // Botão esquerdo
        if (state == 0) { // Pressionado
            Vector2 mousePos(x, y);

            // Verifica se clicou em um ponto existente
            selectedPoint = bezierCurve.findNearestControlPoint(mousePos, Constants::SELECTION_THRESHOLD);

            if (selectedPoint != -1) {
                isDragging = true;
            } else {
                // Adiciona um novo ponto de controle na posição do mouse
                bezierCurve.addControlPoint(mousePos);
                selectedPoint = bezierCurve.getControlPointCount() - 1;
            }
        } else if (state == 1) { // Liberado
            deselectPoint();
        }
    } else if (button == 2 && state == 0) { // Botão direito pressionado
        removeLastControlPoint();
    }
}

void Editor::handleMouseMotion(int x, int y)
{
    // Verifica se está arrastando um ponto de controle
    if (isDragging && selectedPoint != -1) {
        Vector2 newPos(x, y);
        bezierCurve.updateControlPoint(selectedPoint, newPos);
    }
}

void Editor::handleKeyboard(int key)
{
    printf("\nTecla pressionada: %d", key);

    if (key == 82 || key == 114) { // R ou r
        resetCurve();
        printf("\nCurva resetada");
    }
}

void Editor::handleKeyboardUp(int key) { printf("\nTecla liberada: %d", key); }

void Editor::resetCurve() { initializeCurve(); }

void Editor::addControlPoint(const Vector2 &point) { bezierCurve.addControlPoint(point); }

void Editor::removeLastControlPoint()
{
    if (bezierCurve.getControlPointCount() > 2) {
        bezierCurve.removeControlPoint(bezierCurve.getControlPointCount() - 1);
        deselectPoint();
    } else {
        printf("\nNão é possível remover: mínimo de 2 pontos necessário");
    }
}

bool Editor::selectControlPoint(const Vector2 &point)
{
    selectedPoint = bezierCurve.findNearestControlPoint(point, Constants::SELECTION_THRESHOLD);
    return selectedPoint != -1;
}

void Editor::moveSelectedPoint(const Vector2 &newPosition)
{
    if (selectedPoint != -1) {
        bezierCurve.updateControlPoint(selectedPoint, newPosition);
    }
}

void Editor::deselectPoint()
{
    selectedPoint = -1;
    isDragging = false;
}

bool Editor::isEmpty() const { return bezierCurve.isEmpty(); }
