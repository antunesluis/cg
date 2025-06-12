#include "Editor.h"
#include "gl_canvas2d.h"
#include <stdio.h>

Editor::Editor(int width, int height)
    : isDragging(false), selectedPoint(-1), viewer3D(Constants::PANEL_WIDTH, 0, Constants::PANEL_WIDTH, height)
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

    // bezierCurve.addControlPoint(Vector2(Constants::PANEL_WIDTH * 0.3f, screenHeight * 0.7f));
    // bezierCurve.addControlPoint(Vector2(Constants::PANEL_WIDTH * 0.4f, screenHeight * 0.3f));
    // bezierCurve.addControlPoint(Vector2(Constants::PANEL_WIDTH * 0.6f, screenHeight * 0.4f));
    // bezierCurve.addControlPoint(Vector2(Constants::PANEL_WIDTH * 0.7f, screenHeight * 0.8f));

    updateViewer3D();
}

bool Editor::isIn2DPanel(int x, int y) const
{
    return x >= 0 && x < Constants::PANEL_WIDTH && y >= 0 && y < screenHeight;
}

bool Editor::isIn3DPanel(int x, int y) const
{
    return x >= Constants::PANEL_WIDTH && x < screenWidth && y >= 0 && y < screenHeight;
}

void Editor::updateViewer3D()
{
    if (!bezierCurve.isEmpty()) {
        // Força atualização da curva
        bezierCurve.updateCurvePoints();
        std::vector<Vector2> curvePoints = bezierCurve.getCurvePoints();

        if (!curvePoints.empty()) {
            viewer3D.updateObject(curvePoints);
        }
    } else {
        viewer3D.updateObject({});
    }
}

void Editor::render()
{
    CV::clear(0, 0, 0);

    // painel da direita
    viewer3D.render();

    // painel da esquerda
    CV::color(0.05f, 0.05f, 0.1f);
    CV::rectFill(0, 0, Constants::PANEL_WIDTH, screenHeight);

    if (!bezierCurve.isEmpty()) {
        bezierCurve.drawControlPoints(selectedPoint);
        bezierCurve.drawCurve();
    }

    // Desenha informações e linha divisória
    drawInfo();
    drawDivisionLine();
    drawAxes2D();
}

void Editor::drawAxes2D()
{
    CV::color(0.5f, 0.5f, 0.5f);
    CV::line(0, screenHeight / 2, Constants::PANEL_WIDTH, screenHeight / 2);
    CV::color(0.5f, 0.5f, 0.5f);
    CV::line(Constants::PANEL_WIDTH / 2, 0, Constants::PANEL_WIDTH / 2, screenHeight - 80);
}

void Editor::update()
{
    // Atualiza o viewer 3D sempre que necessário
    bezierCurve.updateCurvePoints();
}

void Editor::drawInfo()
{
    char info[400];

    // Informações do painel 2D
    sprintf(info, "2D: Pontos: %zu | Grau: %zu | Curva: %zu pontos", bezierCurve.getControlPointCount(),
            bezierCurve.getControlPointCount() > 0 ? bezierCurve.getControlPointCount() - 1 : 0,
            bezierCurve.getCurvePointCount());

    CV::color(1, 1, 1);
    CV::text(10, screenHeight - 70, info);

    // Informações do painel 3D
    sprintf(info, "3D: WASD=Rotacao | QE=Zoom | R=Reset | RotX:%.1f° RotY:%.1f° Zoom:%.2f",
            viewer3D.getRotationX() * 180.0f / 3.14159f, viewer3D.getRotationY() * 180.0f / 3.14159f,
            viewer3D.getZoom());
    CV::text(Constants::PANEL_WIDTH + 10, screenHeight - 70, info);

    // Instruções gerais
    CV::color(0.8f, 0.8f, 0.8f);
    CV::text(10, screenHeight - 50, "Mouse: Clique=Adicionar/Selecionar | Arrastar=Mover | Direito=Remover");
    CV::text(10, screenHeight - 30, "Teclas: R=Reset | WASD=Rotacao 3D | QE=Zoom 3D");
}

void Editor::drawDivisionLine()
{
    CV::color(0.5f, 0.5f, 0.5f);
    CV::line(Constants::PANEL_WIDTH, 0, Constants::PANEL_WIDTH, screenHeight);
}

void Editor::onMouse(int button, int state, int wheel, int direction, int x, int y)
{
    // Movimento do mouse
    if (button == -2 && state == -2) {
        handleMouseMotion(x, y);
    } else {
        handleMouseClick(button, state, x, y);
    }
}

void Editor::handleMouseClick(int button, int state, int x, int y)
{
    // Verifica se o clique foi no painel 2D
    if (isIn2DPanel(x, y)) {
        if (button == 0) {    // Botão esquerdo
            if (state == 0) { // Pressionado
                Vector2 mousePos(x, y);

                // Verifica se clicou em um ponto existente
                selectedPoint = bezierCurve.findNearestControlPoint(mousePos, Constants::SELECTION_THRESHOLD);

                if (selectedPoint != -1) {
                    isDragging = true;
                } else {
                    // Adiciona um novo ponto de controle
                    bezierCurve.addControlPoint(mousePos);
                    selectedPoint = bezierCurve.getControlPointCount() - 1;
                    updateViewer3D();
                }
            } else if (state == 1) { // Liberado
                deselectPoint();
            }
        } else if (button == 2 && state == 0) { // Botão direito pressionado
            removeLastControlPoint();
        }
    }
    // Verifica se o clique foi no painel 3D
    else if (isIn3DPanel(x, y)) {
        viewer3D.handleMouse(button, state, x, y);
    }
}

void Editor::handleMouseMotion(int x, int y)
{
    // Verifica se está arrastando um ponto no painel 2D
    if (isDragging && selectedPoint != -1 && isIn2DPanel(x, y)) {
        Vector2 newPos(x, y);
        bezierCurve.updateControlPoint(selectedPoint, newPos);
        updateViewer3D();
    } else if (isIn3DPanel(x, y)) {
        // Se estiver no painel 3D, passa o movimento para o viewer
        viewer3D.handleMouseMotion(x, y);
    }
}

void Editor::handleKeyboard(int key)
{
    if (key == 'r' || key == 'R') {
        resetCurve();
    } else {
        // Passa as teclas para o viewer 3D
        viewer3D.handleKeyboard(key);
    }
}

void Editor::handleKeyboardUp(int key)
{
    //
}

void Editor::resetCurve() { initializeCurve(); }

void Editor::addControlPoint(const Vector2 &point)
{
    bezierCurve.addControlPoint(point);
    updateViewer3D();
}

void Editor::removeLastControlPoint()
{
    if (bezierCurve.getControlPointCount() > 2) {
        bezierCurve.removeControlPoint(bezierCurve.getControlPointCount() - 1);
        deselectPoint();
        updateViewer3D();
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
        updateViewer3D();
    }
}

void Editor::deselectPoint()
{
    selectedPoint = -1;
    isDragging = false;
}

bool Editor::isEmpty() const { return bezierCurve.isEmpty(); }
