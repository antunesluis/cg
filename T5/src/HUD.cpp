/**
 * HUD.cpp
 *
 * Implementação do sistema de HUD
 * Renderiza informações usando OpenGL em projeção ortográfica
 */

#include "HUD.h"
#include <iomanip>
#include <sstream>

HUD::HUD() : visible(true), fps(0), screenWidth(1280), screenHeight(720)
{
    // Configurações padrão
    textColor[0] = 1.0f;
    textColor[1] = 1.0f;
    textColor[2] = 1.0f; // Branco
    crosshairColor[0] = 0.0f;
    crosshairColor[1] = 1.0f;
    crosshairColor[2] = 0.0f; // Verde
    backgroundColor[0] = 0.0f;
    backgroundColor[1] = 0.0f;
    backgroundColor[2] = 0.0f;
    backgroundColor[3] = 0.5f; // Semi-transparente

    textStartX = 10;
    textStartY = 30;
    lineSpacing = 20;
}

HUD::HUD(int width, int height) : HUD() { setScreenSize(width, height); }

void HUD::setScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void HUD::setTextColor(float r, float g, float b)
{
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
}

void HUD::setCrosshairColor(float r, float g, float b)
{
    crosshairColor[0] = r;
    crosshairColor[1] = g;
    crosshairColor[2] = b;
}

void HUD::setBackgroundColor(float r, float g, float b, float a)
{
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
    backgroundColor[3] = a;
}

void HUD::setupOrthographicProjection()
{
    // Salvar estado atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Configurar projeção ortográfica
    gluOrtho2D(0, screenWidth, screenHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desabilitar depth test para HUD
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void HUD::restorePerspectiveProjection()
{
    // Restaurar estados
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void HUD::renderText(float x, float y, const std::string &text)
{
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void HUD::renderBackground()
{
    // Renderizar fundo semi-transparente para o texto
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(220, 0);
    glVertex2f(220, 140);
    glVertex2f(0, 140);
    glEnd();

    glDisable(GL_BLEND);
}

void HUD::renderCrosshair()
{
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float size = 10.0f;

    glColor3f(crosshairColor[0], crosshairColor[1], crosshairColor[2]);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    // Linha horizontal
    glVertex2f(centerX - size, centerY);
    glVertex2f(centerX + size, centerY);

    // Linha vertical
    glVertex2f(centerX, centerY - size);
    glVertex2f(centerX, centerY + size);
    glEnd();

    // Círculo central
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(centerX, centerY);
    glEnd();

    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void HUD::renderInfo(const Vector3 &cameraPos, bool wireframeMode, int asteroidCount)
{
    glColor3f(textColor[0], textColor[1], textColor[2]);

    int currentY = textStartY;

    // FPS
    std::ostringstream oss;
    oss << "FPS: " << std::fixed << std::setprecision(1) << fps;
    renderText(textStartX, currentY, oss.str());
    currentY += lineSpacing;

    // Modo de renderização
    std::string mode = wireframeMode ? "Wireframe" : "Solid";
    renderText(textStartX, currentY, "Mode: " + mode);
    currentY += lineSpacing;

    // Posição da câmera
    oss.str("");
    oss << "Pos: (" << std::fixed << std::setprecision(1) << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z
        << ")";
    renderText(textStartX, currentY, oss.str());
    currentY += lineSpacing;

    // Número de asteroides
    oss.str("");
    oss << "Asteroids: " << asteroidCount;
    renderText(textStartX, currentY, oss.str());
    currentY += lineSpacing;

    // Controles (texto menor)
    currentY += 10;
    renderText(textStartX, currentY, "Controls:");
    currentY += 15;
    renderText(textStartX, currentY, "H - Toggle HUD");
    currentY += 15;
    renderText(textStartX, currentY, "F - Toggle Wireframe");
}

void HUD::render(const Vector3 &cameraPos, bool wireframeMode, int asteroidCount, float currentFPS)
{
    if (!visible) {
        // Renderizar apenas o crosshair mesmo com HUD desligado
        setupOrthographicProjection();
        renderCrosshair();
        restorePerspectiveProjection();
        return;
    }

    fps = currentFPS;

    setupOrthographicProjection();

    renderBackground();
    renderInfo(cameraPos, wireframeMode, asteroidCount);
    renderCrosshair();

    restorePerspectiveProjection();
}
