#ifndef BEZIER_MATH_H
#define BEZIER_MATH_H

#include "Vector2.h"

namespace BezierMath {
// Calcula um ponto na curva de Bézier cúbica
static Vector2 calculatePoint(float t, const Vector2 &p0, const Vector2 &p1,
                              const Vector2 &p2, const Vector2 &p3) {
  float u = 1.0f - t;
  float tt = t * t;
  float uu = u * u;
  float uuu = uu * u;
  float ttt = tt * t;

  Vector2 p = p0 * uuu;
  p += p1 * (3 * uu * t);
  p += p2 * (3 * u * tt);
  p += p3 * ttt;

  return p;
}

// Calcula a tangente da curva em um ponto
static Vector2 calculateTangent(float t, const Vector2 &p0, const Vector2 &p1,
                                const Vector2 &p2, const Vector2 &p3) {
  float u = 1.0f - t;
  float uu = u * u;
  float tt = t * t;

  Vector2 tangent = p0 * (-3 * uu);
  tangent += p1 * (3 * uu - 6 * u * t);
  tangent += p2 * (6 * u * t - 3 * tt);
  tangent += p3 * (3 * tt);

  return tangent;
}

// Calcula a normal à curva
static Vector2 calculateNormal(const Vector2 &tangent) {
  Vector2 normal = tangent;
  normal.normalize();
  return Vector2(-normal.y, normal.x); // Rotação de 90 graus
}
} // namespace BezierMath

#endif
