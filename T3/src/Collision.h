#ifndef COLLISION_H
#define COLLISION_H

#include "Vector2.h"
#include <vector>

class Collision {
public:
  // Colisão círculo-círculo (para projéteis e alvos)
  static bool circleCircle(const Vector2 &pos1, float radius1,
                           const Vector2 &pos2, float radius2) {
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = radius1 + radius2;
    return distanceSquared <= (radiusSum * radiusSum);
  }

  // Colisão ponto-círculo
  static bool pointCircle(const Vector2 &point, const Vector2 &circlePos,
                          float radius) {
    return circleCircle(point, 0, circlePos, radius);
  }

  // Colisão retângulo-círculo (para tanque com bordas)
  static bool rectCircle(const Vector2 &rectPos, float width, float height,
                         const Vector2 &circlePos, float radius) {
    // Encontra o ponto mais próximo do círculo no retângulo
    float closestX = std::max(rectPos.x - width / 2,
                              std::min(circlePos.x, rectPos.x + width / 2));
    float closestY = std::max(rectPos.y - height / 2,
                              std::min(circlePos.y, rectPos.y + height / 2));

    // Calcula a distância
    float dx = circlePos.x - closestX;
    float dy = circlePos.y - closestY;
    return (dx * dx + dy * dy) < (radius * radius);
  }
};

#endif
