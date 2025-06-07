#ifndef TARGET_H
#define TARGET_H

#include "Vector2.h"
#include "gl_canvas2d.h"
#include <vector>

class Target
{
  private:
    Vector2 position;
    Vector2 direction;   // direção do movimento
    float speed = 30.0f; // Velocidade do alvo
    float radius = 20.0f;
    float health = 100.0f;
    bool destroyed = false;
    int currentPathIndex = 0; // Índice do ponto da pista que o alvo está seguindo

  public:
    Target(float x, float y) : position(x, y) {}

    void takeDamage(float amount)
    {
        health -= amount;
        if (health <= 0)
            destroyed = true;
    }
    void update(float deltaTime, const std::vector<Vector2> &pathPoints)
    {
        if (destroyed || pathPoints.empty())
            return;

        // Move em direção ao próximo ponto da pista
        Vector2 targetPoint = pathPoints[currentPathIndex];
        direction = (targetPoint - position);
        if (direction.length() > 0.01f) {
            direction.normalize();
            position += direction * speed * deltaTime;
        }

        // Verifica se chegou ao ponto atual e avança para o próximo
        if ((position - targetPoint).length() < 10.0f) {
            currentPathIndex = (currentPathIndex + 1) % pathPoints.size();
        }
    }

    void render() const
    {
        if (destroyed)
            return;

        // Cor baseada na saúde
        float healthRatio = health / 100.0f;
        CV::color(healthRatio, 0, 1 - healthRatio);
        CV::circleFill(position.x, position.y, radius, 20);

        // Barra de vida
        CV::color(1, 1, 1);
        CV::rect(position.x - radius, position.y - radius - 10, position.x + radius, position.y - radius - 5);
        CV::color(1 - healthRatio, healthRatio, 0);
        CV::rectFill(position.x - radius, position.y - radius - 10, position.x - radius + 2 * radius * healthRatio,
                     position.y - radius - 5);
    }

    void reset(float x, float y)
    {
        position.set(x, y);
        health = 100.0f;
        destroyed = false;
    }

    bool isDestroyed() const { return destroyed; }
    const Vector2 &getPosition() const { return position; }
    float getRadius() const { return radius; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void setHealth(float newHealth) { health = newHealth; }
    void resetPathIndex() { currentPathIndex = 0; }
};
#endif
