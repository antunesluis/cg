#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>
#include <cstdio>

class Vector2
{
  public:
    float x, y;

    // Construtores
    Vector2();
    Vector2(float _x, float _y);

    // Métodos
    void set(float _x, float _y);
    void normalize();

    // Operadores
    Vector2 operator-(const Vector2 &v) const;
    Vector2 operator+(const Vector2 &v) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    bool operator==(const Vector2 &v) const;

    // Métodos estáticos
    static float dot(const Vector2 &a, const Vector2 &b);
    static float distance(const Vector2 &a, const Vector2 &b);
};

#endif // VECTOR_2_H
