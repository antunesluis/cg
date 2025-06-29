/**
 * Asteroid.h
 *
 * Classe responsável pelos asteroides da cena
 * Inclui LOD (Level of Detail) baseado na distância da câmera
 */

#ifndef ASTEROID_H
#define ASTEROID_H

#include "Vector3.h"
#include <GL/glut.h>

enum AsteroidType
{
    SPHERE,
    CUBE,
    TORUS,
    TEAPOT,
    ICOSAHEDRON
};

class Asteroid
{
  private:
    Vector3 position;
    Vector3 rotation;      // Ângulos de rotação atuais
    Vector3 rotationSpeed; // Velocidade de rotação em cada eixo
    float scale;
    AsteroidType type;

    // LOD - Level of Detail
    int detailLevel;       // 0 = baixo, 1 = médio, 2 = alto
    float lodDistances[3]; // Distâncias para cada nível de LOD

    // Cor do asteroide
    float color[3];

    void renderSphere(int detail);
    void renderCube();
    void renderTorus(int detail);
    void renderTeapot(bool solid);
    void renderIcosahedron();

    void calculateNormals(); // Para iluminação adequada

  public:
    Asteroid();
    Asteroid(const Vector3 &pos, float size, AsteroidType asteroidType);

    // Getters
    Vector3 getPosition() const { return position; }
    float getScale() const { return scale; }
    AsteroidType getType() const { return type; }

    // Setters
    void setPosition(const Vector3 &pos) { position = pos; }
    void setScale(float size) { scale = size; }
    void setRotationSpeed(const Vector3 &speed) { rotationSpeed = speed; }
    void setColor(float r, float g, float b);

    // Atualização
    void update(float deltaTime);

    // Renderização
    void render(const Vector3 &cameraPos, bool wireframe = false);

    // LOD
    void updateLOD(const Vector3 &cameraPos);
    int calculateDetailLevel(float distance) const;

    // Utilidades
    float distanceToCamera(const Vector3 &cameraPos) const;
    bool isVisible(const Vector3 &cameraPos, float maxDistance = 200.0f) const;

    // Geração procedural
    static AsteroidType getRandomType();
    void randomizeProperties();
};

#endif
