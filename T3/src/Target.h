#ifndef TARGET_H
#define TARGET_H

#include "Vector2.h"
#include "gl_canvas2d.h"

class Target {
private:
  Vector2 position;
  float radius = 20.0f;
  float health = 100.0f;
  bool destroyed = false;

public:
  Target(float x, float y) : position(x, y) {}

  void takeDamage(float amount) {
    health -= amount;
    if (health <= 0)
      destroyed = true;
  }

  void render() const {
    if (destroyed)
      return;

    // Cor baseada na saúde
    float healthRatio = health / 100.0f;
    CV::color(healthRatio, 0, 1 - healthRatio);
    CV::circleFill(position.x, position.y, radius, 20);

    // Barra de vida
    CV::color(1, 1, 1);
    CV::rect(position.x - radius, position.y - radius - 10, position.x + radius,
             position.y - radius - 5);
    CV::color(1 - healthRatio, healthRatio, 0);
    CV::rectFill(position.x - radius, position.y - radius - 10,
                 position.x - radius + 2 * radius * healthRatio,
                 position.y - radius - 5);
  }

  void reset(float x, float y) {
    position.set(x, y);
    health = 100.0f;
    destroyed = false;
  }

  bool isDestroyed() const { return destroyed; }
  const Vector2 &getPosition() const { return position; }
  float getRadius() const { return radius; }
};
#endif
