/**
 * HUD.h
 *
 * Sistema de interface gráfica para exibição de informações em tempo real
 * Inclui FPS, coordenadas da câmera, modo de renderização e crosshair
 */

#ifndef HUD_H
#define HUD_H

#include "Vector3.h"
#include <GL/glut.h>
#include <string>

class HUD
{
  private:
    bool visible;
    float fps;
    int screenWidth, screenHeight;

    float textColor[3];
    float crosshairColor[3];
    float backgroundColor[4];

    int textStartX, textStartY;
    int lineSpacing;

    void setupOrthographicProjection();
    void restorePerspectiveProjection();
    void renderText(float x, float y, const std::string &text);
    void renderBackground();
    void renderCrosshair();
    void renderInfo(const Vector3 &cameraPos, bool wireframeMode, int asteroidCount);

  public:
    HUD();
    HUD(int width, int height);

    void setScreenSize(int width, int height);
    void setVisible(bool show) { visible = show; }
    void setFPS(float newFPS) { fps = newFPS; }

    void setTextColor(float r, float g, float b);
    void setCrosshairColor(float r, float g, float b);
    void setBackgroundColor(float r, float g, float b, float a);

    void render(const Vector3 &cameraPos, bool wireframeMode, int asteroidCount, float currentFPS);

    void toggle() { visible = !visible; }
    bool isVisible() const { return visible; }
};

#endif
