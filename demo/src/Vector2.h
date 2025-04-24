#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <math.h>
#include <stdio.h>

class Vector2
{
  public:
    float x, y;

    Vector2() { x = y = 0; }

    Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void normalize()
    {
        float norm = (float)sqrt(x * x + y * y);

        if (norm == 0.0) {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    Vector2 operator-(const Vector2 &v)
    {
        Vector2 aux(x - v.x, y - v.y);
        return (aux);
    }

    Vector2 operator+(const Vector2 &v)
    {
        Vector2 aux(x + v.x, y + v.y);
        return (aux);
    }

    // Multiplicação por escalar
    Vector2 operator*(const float scalar)
    {
        Vector2 aux(x * scalar, y * scalar);
        return (aux);
    }

    // Divisão por escalar
    Vector2 operator/(const float scalar)
    {
        if (scalar == 0.0f) {
            printf("\n\nErro: Divisão por zero");
            return Vector2(0, 0);
        }
        Vector2 aux(x / scalar, y / scalar);
        return (aux);
    }

    // Produto escalar (produto interno)
    float operator*(const Vector2 &v) { return (x * v.x + y * v.y); }

    // Operadores de atribuição
    Vector2 &operator+=(const Vector2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2 &operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2 &operator/=(const float scalar)
    {
        if (scalar == 0.0f) {
            printf("\n\nErro: Divisão por zero");
            return *this;
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Operadores de igualdade
    bool operator==(const Vector2 &v) { return (x == v.x && y == v.y); }

    bool operator!=(const Vector2 &v) { return (x != v.x || y != v.y); }

    // Comprimento/magnitude do vetor
    float length() const { return sqrt(x * x + y * y); }
};

#endif
