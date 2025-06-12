#include "Viewer3D.h"
#include "Constants.h"
#include "gl_canvas2d.h"
#include <math.h>
#include <stdio.h>

Viewer3D::Viewer3D(int x, int y, int width, int height)
    : viewportX(x), viewportY(y), viewportWidth(width), viewportHeight(height), rotationX(-0.3f), rotationY(0.3f),
      zoom(1.5f), offset(width / 2, height / 2), object(Constants::DEFAULT_ROTATION_STEPS)
{
}

void Viewer3D::setViewport(int x, int y, int width, int height)
{
    viewportX = x;
    viewportY = y;
    viewportWidth = width;
    viewportHeight = height;
    offset.set(width / 2, height / 2);
}

void Viewer3D::updateObject(const std::vector<Vector2> &profile)
{
    // Converte coordenadas da tela para coordenadas do mundo 3D
    std::vector<Vector2> worldProfile;
    worldProfile.reserve(profile.size());

    for (const Vector2 &point : profile) {
        // Converte coordenadas da tela para coordenadas do mundo
        float worldX = (point.x - Constants::PANEL_WIDTH / 2) * 0.3f;
        float worldY = -(point.y - Constants::SCREEN_HEIGHT / 2) * 0.3f; // Inverte Y
        worldProfile.emplace_back(fabs(worldX), worldY);                 // Garante que X seja positivo (raio)
    }

    object.updateProfile(worldProfile);
}

Vector2 Viewer3D::projectPoint(const Vector3 &point3D) const
{
    // Aplicar rotações da câmera
    float cosX = cos(rotationX);
    float sinX = sin(rotationX);
    float cosY = cos(rotationY);
    float sinY = sin(rotationY);

    // Rotação em X (pitch)
    float y1 = point3D.y * cosX - point3D.z * sinX;
    float z1 = point3D.y * sinX + point3D.z * cosX;

    // Rotação em Y (yaw)
    float x2 = point3D.x * cosY + z1 * sinY;
    float z2 = -point3D.x * sinY + z1 * cosY;

    // Projeção perspectiva simples
    float perspective = 1.0f + z2 * 0.001f; // Pequeno efeito perspectivo
    if (perspective < 0.1f)
        perspective = 0.1f;

    float screenX = viewportX + offset.x + (x2 * zoom) / perspective;
    float screenY = viewportY + offset.y - (y1 * zoom) / perspective;

    return Vector2(screenX, screenY);
}

void Viewer3D::drawAxes() const
{
    const float axisLength = 80.0f;
    Vector3 origin(0, 0, 0);
    Vector3 axisX(axisLength, 0, 0);
    Vector3 axisY(0, axisLength, 0);
    Vector3 axisZ(0, 0, axisLength);

    Vector2 originProj = projectPoint(origin);
    Vector2 axisXProj = projectPoint(axisX);
    Vector2 axisYProj = projectPoint(axisY);
    Vector2 axisZProj = projectPoint(axisZ);

    // Eixo X - vermelho
    CV::color(1, 0, 0);
    CV::line(originProj.x, originProj.y, axisXProj.x, axisXProj.y);
    CV::text(axisXProj.x + 5, axisXProj.y, "X");

    // Eixo Y - verde
    CV::color(0, 1, 0);
    CV::line(originProj.x, originProj.y, axisYProj.x, axisYProj.y);
    CV::text(axisYProj.x + 5, axisYProj.y, "Y");

    // Eixo Z - azul
    CV::color(0, 0, 1);
    CV::line(originProj.x, originProj.y, axisZProj.x, axisZProj.y);
    CV::text(axisZProj.x + 5, axisZProj.y, "Z");
}

void Viewer3D::drawGrid() const
{
    CV::color(0.2f, 0.2f, 0.2f); // Cinza bem escuro para a grade

    const float gridSize = 20.0f;
    const float gridRange = 100.0f;

    // Grade no plano XZ (horizontal)
    for (float i = -gridRange; i <= gridRange; i += gridSize) {
        // Linhas paralelas ao eixo X
        Vector2 start = projectPoint(Vector3(-gridRange, 0, i));
        Vector2 end = projectPoint(Vector3(gridRange, 0, i));
        CV::line(start.x, start.y, end.x, end.y);

        // Linhas paralelas ao eixo Z
        start = projectPoint(Vector3(i, 0, -gridRange));
        end = projectPoint(Vector3(i, 0, gridRange));

        CV::line(start.x, start.y, end.x, end.y);
    }
}

void Viewer3D::render()
{
    CV::color(0.1f, 0.1f, 0.1f);
    CV::rectFill(viewportX, viewportY, viewportX + viewportWidth, viewportY + viewportHeight);

    drawGrid();
    drawAxes();

    if (!object.isEmpty()) {
        const auto &vertices = object.getVertices();

        if (!vertices.empty()) {
            CV::color(1, 0, 1); // Amarelo para o objeto 3D

            int rotationSteps = object.getRotationSteps();
            size_t profileSize = object.getProfileSize();

            // Desenha linhas verticais (ao longo da curva de perfil)
            for (int i = 0; i < rotationSteps; i++) {
                if (vertices[i].size() > 1) {
                    for (size_t j = 1; j < vertices[i].size(); j++) {
                        Vector2 p1 = projectPoint(vertices[i][j - 1]);
                        Vector2 p2 = projectPoint(vertices[i][j]);
                        CV::line(p1.x, p1.y, p2.x, p2.y);
                    }
                }
            }

            // Desenha linhas horizontais (entre rotações)
            for (size_t j = 0; j < profileSize; j++) {
                for (int i = 0; i < rotationSteps; i++) {
                    int nextI = (i + 1) % rotationSteps;

                    if (j < vertices[i].size() && j < vertices[nextI].size()) {
                        Vector2 p1 = projectPoint(vertices[i][j]);
                        Vector2 p2 = projectPoint(vertices[nextI][j]);
                        CV::line(p1.x, p1.y, p2.x, p2.y);
                    }
                }
            }
        }
    }
}

void Viewer3D::handleMouse(int button, int state, int x, int y)
{
    //
}

void Viewer3D::handleMouseMotion(int x, int y)
{
    //
}

void Viewer3D::handleKeyboard(int key)
{
    const float rotationStep = 0.15f;
    const float zoomStep = 0.2f;

    switch (key) {
    case 'w':
    case 'W':
        rotationX += rotationStep;
        break;
    case 's':
    case 'S':
        rotationX -= rotationStep;
        break;
    case 'a':
    case 'A':
        rotationY += rotationStep;
        break;
    case 'd':
    case 'D':
        rotationY -= rotationStep;
        break;
    case 'q':
    case 'Q':
        zoomCamera(1.0f);
        break;
    case 'e':
    case 'E':
        zoomCamera(-1.0f);
        break;
    case 'r':
    case 'R':
        resetCamera();
        break;
    }
}

void Viewer3D::rotateCamera(float deltaX, float deltaY)
{
    rotationY += deltaX * Constants::ROTATION_SENSITIVITY;
    rotationX += deltaY * Constants::ROTATION_SENSITIVITY;

    // Limita a rotação em X
    if (rotationX > M_PI / 2)
        rotationX = M_PI / 2;
    if (rotationX < -M_PI / 2)
        rotationX = -M_PI / 2;
}

void Viewer3D::zoomCamera(float delta)
{
    zoom += delta * 0.1f;
    if (zoom < 0.3f)
        zoom = 0.3f;
    if (zoom > 4.0f)
        zoom = 4.0f;
}

void Viewer3D::resetCamera()
{
    rotationX = -0.3f;
    rotationY = 0.3f;
    zoom = 1.5f;
}
