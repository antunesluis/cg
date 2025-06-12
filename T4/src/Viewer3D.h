#ifndef __VIEWER_3D_H__
#define __VIEWER_3D_H__

#include "Object3D.h"
#include "Vector2.h"
#include "Vector3.h"
#include <cstdio>

class Viewer3D
{
  private:
    Object3D object;

    // Parâmetros da câmera/visualização
    float rotationX;
    float rotationY;
    float zoom;
    Vector2 offset;

    // Área de visualização
    int viewportX, viewportY;
    int viewportWidth, viewportHeight;

    bool wireframeMode;
    bool surfaceMode;
    bool normalsMode;
    bool shadowsEnabled;
    Vector3 lightDirection;

    // Projeção
    Vector2 projectPoint(const Vector3 &point3D) const;
    void drawAxes() const;
    void drawGrid() const;
    void drawShadows(const std::vector<Triangle3D> &triangles) const;
    Vector3 projectPointOnPlane(const Vector3 &point, const Vector3 &lightDir, const Vector3 &planeNormal,
                                float planeD) const;

    void drawTriangle(const Triangle3D &triangle) const;
    void drawWireframe(const Triangle3D &triangle) const;
    void drawNormal(const Triangle3D &triangle) const;

  public:
    Viewer3D(int x, int y, int width, int height);

    void setViewport(int x, int y, int width, int height);
    void updateObject(const std::vector<Vector2> &profile);

    void render();
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void handleKeyboard(int key);

    // Controles da câmera
    void rotateCamera(float deltaX, float deltaY);
    void zoomCamera(float delta);
    void resetCamera();
    void toggleShadows() { shadowsEnabled = !shadowsEnabled; }
    void setLightDirection(const Vector3 &dir) { lightDirection = dir.normalized(); }

    bool isEmpty() const { return object.isEmpty(); }

    // Getters para os parâmetros
    float getRotationX() const { return rotationX; }
    float getRotationY() const { return rotationY; }
    float getZoom() const { return zoom; }
    const Vector3 &getLightDirection() const { return lightDirection; }

    void setWireframeMode(bool enabled) { wireframeMode = enabled; }
    void setSurfaceMode(bool enabled) { surfaceMode = enabled; }
    void setNormalsMode(bool enabled) { normalsMode = enabled; }

    void setRotationSteps(int steps) { object.setRotationSteps(steps); }
};

#endif
