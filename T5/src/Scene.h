/**
 * Scene.h
 *
 * Classe responsável pelo gerenciamento da cena completa
 * Controla asteroides, iluminação e renderização geral
 */

#ifndef SCENE_H
#define SCENE_H

#include "Asteroid.h"
#include "Vector3.h"
#include <GL/glut.h>
#include <vector>

class Scene
{
  private:
    std::vector<Asteroid *> asteroids;

    // Parâmetros da cena
    float sceneSize;   // Tamanho da área de asteroides
    int asteroidCount; // Número de asteroides

    // Iluminação
    bool lightingEnabled;

    // Skybox/Background
    void renderSkybox();
    void renderStars();

    // Utilitários
    Vector3 generateRandomPosition(float range);
    bool checkCollision(const Vector3 &pos, float radius);

  public:
    Scene();
    ~Scene();

    // Configuração da cena
    void generateAsteroids(int count, float range);
    void clearAsteroids();

    // Atualização e renderização
    void update(float deltaTime);
    void render(const Vector3 &cameraPos, bool wireframe = false);

    // Getters/Setters
    int getAsteroidCount() const { return asteroids.size(); }
    float getSceneSize() const { return sceneSize; }

    void setLighting(bool enabled) { lightingEnabled = enabled; }
    bool isLightingEnabled() const { return lightingEnabled; }

    // Utilitários da cena
    Asteroid *getNearestAsteroid(const Vector3 &position);
    std::vector<Asteroid *> getAsteroidsInRange(const Vector3 &position, float range);
};

#endif
