#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2.h"
#include "gl_canvas2d.h"

class Projectile {
private:
  Vector2 position;
  Vector2 direction;
  float speed;
  float radius = 8.0f;
  float lifetime = 2.0f; // Tempo em segundos antes de desaparecer
  float timeAlive = 0.0f;

  bool shouldBeDestroyed = false;

public:
  Projectile(const Vector2 &startPos, const Vector2 &dir, float spd)
      : position(startPos), direction(dir), speed(spd) {
    direction.normalize();
  }

  void update(float deltaTime) {
    position += direction * (speed * deltaTime);
    timeAlive += deltaTime;
  }

  void render() const {
    CV::color(1.0f, 0.0f, 0.0f); // Vermelho
    CV::circleFill(position.x, position.y, radius, 12);
  }

  void markForDestruction() { shouldBeDestroyed = true; }

  // Modificar shouldDestroy
  bool shouldDestroy() const {
    return timeAlive >= lifetime || shouldBeDestroyed;
  }

  const Vector2 &getPosition() const { return position; }
  float getRadius() const { return radius; }
};

#endif
