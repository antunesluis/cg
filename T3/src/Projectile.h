
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Colors.h"
#include "Vector2.h"
#include "gl_canvas2d.h"

class Projectile
{
  private:
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius = 8.0f;
    float lifetime = 2.0f; // Tempo em segundos antes de desaparecer
    float timeAlive = 0.0f;

    bool shouldBeDestroyed = false;

  public:
    Projectile(const Vector2 &startPos, const Vector2 &dir, float spd) : position(startPos), direction(dir), speed(spd)
    {
        direction.normalize();
    }

    void update(float deltaTime)
    {
        position += direction * (speed * deltaTime);
        timeAlive += deltaTime;
    }

    void render() const
    {
        Colors::projectile();
        CV::circleFill(position.x, position.y, radius, 12);

        // Opcional: adicionar efeito visual (rastro)
        CV::color(1.0f, 0.6f, 0.2f, 0.5f); // Cor laranja transparente
        CV::circleFill(position.x - direction.x * 5, position.y - direction.y * 5, radius * 0.7f, 8);
    }

    void markForDestruction() { shouldBeDestroyed = true; }

    // Modificar shouldDestroy
    bool shouldDestroy() const { return timeAlive >= lifetime || shouldBeDestroyed; }

    const Vector2 &getPosition() const { return position; }
    float getRadius() const { return radius; }
};

#endif
