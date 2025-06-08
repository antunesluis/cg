#include "BezierCurve2D.h"
#include "Constants.h"
#include "gl_canvas2d.h"

BCurve2D::BCurve2D() : needsUpdate(true) {}

void BCurve2D::addControlPoint(Vector2 point)
{
    controlPoints.push_back(point);
    needsUpdate = true;
}

void BCurve2D::removeControlPoint(int index)
{
    if (index >= 0 && index < (int)controlPoints.size()) {
        controlPoints.erase(controlPoints.begin() + index);
        needsUpdate = true;
    }
}

void BCurve2D::updateControlPoint(int index, Vector2 newPos)
{
    if (index >= 0 && index < (int)controlPoints.size()) {
        controlPoints[index] = newPos;
        needsUpdate = true;
    }
}

void BCurve2D::updateCurvePoints()
{
    if (!needsUpdate || controlPoints.size() < 2)
        return;

    const int NUM_SAMPLES = Constants::DEFAULT_NUM_SAMPLES;
    curvePoints.clear();
    curvePoints.reserve(NUM_SAMPLES + 1); // Otimização: reserva espaço

    for (int i = 0; i <= NUM_SAMPLES; i++) {
        float t = (float)i / NUM_SAMPLES;
        curvePoints.push_back(calculatePoint(t));
    }
    needsUpdate = false;
}

int BCurve2D::binomialCoefficient(int n, int k) const
{
    if (k > n - k)
        k = n - k; // Otimização: C(n,k) = C(n,n-k)

    int result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

Vector2 BCurve2D::calculatePoint(float t) const
{
    if (controlPoints.size() < 2)
        return Vector2(0, 0);

    int n = controlPoints.size() - 1;
    Vector2 point(0, 0);

    for (int i = 0; i <= n; i++) {
        float binomial = (float)binomialCoefficient(n, i);
        float bernstein = binomial * pow(t, i) * pow(1 - t, n - i);
        point = point + controlPoints[i] * bernstein;
    }

    return point;
}

int BCurve2D::findNearestControlPoint(const Vector2 &point, float threshold) const
{
    for (size_t i = 0; i < controlPoints.size(); i++) {
        float dx = point.x - controlPoints[i].x;
        float dy = point.y - controlPoints[i].y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= threshold) {
            return (int)i;
        }
    }
    return -1;
}

void BCurve2D::drawCurve() const
{
    if (controlPoints.size() < 2)
        return;

    // Força atualização se necessário (método const precisa de cast)
    if (needsUpdate) {
        const_cast<BCurve2D *>(this)->updateCurvePoints();
    }

    if (curvePoints.size() < 2)
        return;

    CV::color(0, 1, 1); // Ciano para a curva
    for (size_t i = 1; i < curvePoints.size(); i++) {
        CV::line(curvePoints[i - 1].x, curvePoints[i - 1].y, curvePoints[i].x, curvePoints[i].y);
    }
}

void BCurve2D::drawControlPoints(int selectedIndex) const
{
    if (controlPoints.size() == 0)
        return;

    const float POINT_RADIUS = Constants::CONTROL_POINT_RADIUS;

    // Desenha linhas entre os pontos de controle (polígono de controle)
    if (controlPoints.size() > 1) {
        CV::color(0.5, 0.5, 0.5); // Cinza para as linhas
        for (size_t i = 0; i < controlPoints.size() - 1; i++) {
            CV::line(controlPoints[i].x, controlPoints[i].y, controlPoints[i + 1].x, controlPoints[i + 1].y);
        }
    }

    // Desenha os pontos de controle
    for (size_t i = 0; i < controlPoints.size(); i++) {
        // Destaca o ponto selecionado
        if ((int)i == selectedIndex) {
            CV::color(1, 0, 0); // Vermelho para ponto selecionado
            CV::circleFill(controlPoints[i].x, controlPoints[i].y, POINT_RADIUS + 2, 20);
        }

        CV::color(1, 1, 0); // Amarelo para pontos normais
        CV::circleFill(controlPoints[i].x, controlPoints[i].y, POINT_RADIUS, 20);

        // Rótulo do ponto
        char label[10];
        sprintf(label, "P%zu", i);
        CV::color(1, 1, 1); // Branco para o texto
        CV::text(controlPoints[i].x + 12, controlPoints[i].y + 12, label);
    }
}

size_t BCurve2D::getControlPointCount() const { return controlPoints.size(); }

size_t BCurve2D::getCurvePointCount() const { return curvePoints.size(); }

void BCurve2D::clearControlPoints()
{
    controlPoints.clear();
    curvePoints.clear();
    needsUpdate = true;
}

bool BCurve2D::isEmpty() const { return controlPoints.empty(); }
