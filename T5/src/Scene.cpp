/**
 * Scene.cpp
 *
 * Implementação da classe Scene
 * Gerencia todos os asteroides e elementos da cena
 */

#include "Scene.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Scene::Scene() : sceneSize(200.0f), asteroidCount(0), lightingEnabled(true) { srand(time(nullptr)); }

Scene::~Scene() { clearAsteroids(); }

void Scene::generateAsteroids(int count, float range)
{
    clearAsteroids();

    sceneSize = range;
    asteroidCount = count;

    for (int i = 0; i < count; i++) {
        Vector3 position = generateRandomPosition(range);

        while (checkCollision(position, 5.0f)) {
            position = generateRandomPosition(range);
        }

        // Gerar tamanho aleatório (0.5 a 3.0)
        float size = 0.5f + ((float)rand() / RAND_MAX) * 2.5f;

        AsteroidType type = Asteroid::getRandomType();
        Asteroid *asteroid = new Asteroid(position, size, type);

        // Adicionar variação na cor baseada no tipo
        switch (type) {
        case SPHERE:
            asteroid->setColor(0.6f, 0.6f, 0.6f); // Cinza
            break;
        case CUBE:
            asteroid->setColor(0.7f, 0.5f, 0.3f); // Marrom
            break;
        case TORUS:
            asteroid->setColor(0.5f, 0.7f, 0.5f); // Verde
            break;
        case TEAPOT:
            asteroid->setColor(0.8f, 0.6f, 0.2f); // Dourado
            break;
        case ICOSAHEDRON:
            asteroid->setColor(0.4f, 0.4f, 0.8f); // Azul
            break;
        }

        asteroids.push_back(asteroid);
    }

    std::cout << "Gerados " << count << " asteroides em área de " << range << "x" << range << "x" << range << std::endl;
}

void Scene::clearAsteroids()
{
    for (Asteroid *asteroid : asteroids) {
        delete asteroid;
    }
    asteroids.clear();
    asteroidCount = 0;
}

Vector3 Scene::generateRandomPosition(float range)
{
    float halfRange = range / 2.0f;

    float x = ((float)rand() / RAND_MAX) * range - halfRange;
    float y = ((float)rand() / RAND_MAX) * range - halfRange;
    float z = ((float)rand() / RAND_MAX) * range - halfRange;

    return Vector3(x, y, z);
}

bool Scene::checkCollision(const Vector3 &pos, float radius)
{
    for (const Asteroid *asteroid : asteroids) {
        Vector3 diff = pos - asteroid->getPosition();
        float distance = diff.length();

        float minDistance = radius + asteroid->getScale() * 2.0f; // Margem de segurança

        if (distance < minDistance) {
            return true;
        }
    }
    return false;
}

void Scene::update(float deltaTime)
{
    for (Asteroid *asteroid : asteroids) {
        asteroid->update(deltaTime);
    }
}

void Scene::render(const Vector3 &cameraPos, bool wireframe)
{
    renderStars();

    // Configurar iluminação
    if (lightingEnabled && !wireframe) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHTING);
    }

    // Renderizar asteroides
    int renderedCount = 0;
    for (Asteroid *asteroid : asteroids) {
        if (asteroid->isVisible(cameraPos, sceneSize)) {
            asteroid->render(cameraPos, wireframe);
            renderedCount++;
        }
    }

    // Debug: mostrar quantos asteroides foram renderizados
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 60 == 0) { // A cada 60 frames (~1 segundo)
        std::cout << "Asteroides renderizados: " << renderedCount << "/" << asteroids.size() << std::endl;
    }
}

void Scene::renderStars()
{
    glDisable(GL_LIGHTING);
    glPointSize(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POINTS);

    srand(12345);
    for (int i = 0; i < 200; i++) {
        float x = ((float)rand() / RAND_MAX) * 400.0f - 200.0f;
        float y = ((float)rand() / RAND_MAX) * 400.0f - 200.0f;
        float z = ((float)rand() / RAND_MAX) * 400.0f - 200.0f;

        // Garantir que as estrelas estejam longe do centro
        Vector3 starPos(x, y, z);
        if (starPos.length() > 150.0f) {
            glVertex3f(x, y, z);
        }
    }

    glEnd();

    // Restaurar seed aleatória
    srand(time(nullptr));
}

void Scene::renderSkybox() { renderStars(); }

Asteroid *Scene::getNearestAsteroid(const Vector3 &position)
{
    if (asteroids.empty())
        return nullptr;

    Asteroid *nearest = asteroids[0];
    float minDistance = (position - nearest->getPosition()).length();

    for (Asteroid *asteroid : asteroids) {
        float distance = (position - asteroid->getPosition()).length();
        if (distance < minDistance) {
            minDistance = distance;
            nearest = asteroid;
        }
    }

    return nearest;
}

std::vector<Asteroid *> Scene::getAsteroidsInRange(const Vector3 &position, float range)
{
    std::vector<Asteroid *> result;

    for (Asteroid *asteroid : asteroids) {
        float distance = (position - asteroid->getPosition()).length();
        if (distance <= range) {
            result.push_back(asteroid);
        }
    }

    return result;
}
