#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <math.h>
#include <stdio.h>

class Vector3
{
  public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    float length() const { return sqrt(x * x + y * y + z * z); }

    void normalize()
    {
        float norm = length();
        if (norm == 0.0f) {
            printf("\nVector3::normalize - Divisao por zero, usando vetor unitario em Y\n");
            x = 0.0f;
            y = 1.0f;
            z = 0.0f;
            return;
        }
        x /= norm;
        y /= norm;
        z /= norm;
    }

    Vector3 normalized() const
    {
        Vector3 result = *this;
        result.normalize();
        return result;
    }

    // Operadores aritméticos (const corretos)
    Vector3 operator+(const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

    Vector3 operator-(const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

    Vector3 operator*(const Vector3 &v) const { return Vector3(x * v.x, y * v.y, z * v.z); }

    Vector3 operator/(float scalar) const
    {
        if (scalar == 0.0f) {
            printf("\nVector3::operator/ - Divisao por zero\n");
            return Vector3(0, 0, 0);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // Operadores de atribuição
    Vector3 &operator+=(const Vector3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 &operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Produto escalar (dot product)
    float dot(const Vector3 &v) const { return x * v.x + y * v.y + z * v.z; }

    // Produto vetorial (cross product)
    Vector3 cross(const Vector3 &v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

    // Operadores de comparação
    bool operator==(const Vector3 &v) const
    {
        const float epsilon = 1e-6f;
        return (fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon && fabs(z - v.z) < epsilon);
    }

    bool operator!=(const Vector3 &v) const { return !(*this == v); }
};

#endif
