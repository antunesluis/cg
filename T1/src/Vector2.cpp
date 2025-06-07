#include "Vector2.h"

// Construtores
Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

// Métodos
void Vector2::set(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Vector2::normalize()
{
    float norm = std::sqrt(x * x + y * y);
    if (norm == 0.0f) {
        printf("\n\nNormalize::Divisao por zero");
        x = 1;
        y = 1;
        return;
    }
    x /= norm;
    y /= norm;
}

// Operadores
Vector2 Vector2::operator-(const Vector2 &v) const { return Vector2(x - v.x, y - v.y); }

Vector2 Vector2::operator+(const Vector2 &v) const { return Vector2(x + v.x, y + v.y); }

Vector2 Vector2::operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

Vector2 Vector2::operator/(float scalar) const
{
    if (scalar == 0.0f) {
        printf("\n\nOperator/::Divisao por zero");
        return *this;
    }
    return Vector2(x / scalar, y / scalar);
}

bool Vector2::operator==(const Vector2 &v) const { return (x == v.x) && (y == v.y); }

// Métodos estáticos
float Vector2::dot(const Vector2 &a, const Vector2 &b) { return a.x * b.x + a.y * b.y; }

float Vector2::distance(const Vector2 &a, const Vector2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
