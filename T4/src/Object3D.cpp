#include "Object3D.h"
#include <math.h>

constexpr double MY_PI = 3.14159265358979323846;

Object3D::Object3D(int steps) : rotationSteps(steps), needsUpdate(true)
{
    vertices.clear();
    triangles.clear();
}

void Object3D::setProfile(const std::vector<Vector2> &curvePoints)
{
    profile = curvePoints;
    needsUpdate = true;
    generateVertices();
    generateTriangles();
}

void Object3D::updateProfile(const std::vector<Vector2> &curvePoints) { setProfile(curvePoints); }

void Object3D::setRotationSteps(int steps)
{
    if (steps < 3)
        steps = 3;
    if (steps > 50)
        steps = 50;
    rotationSteps = steps;
    needsUpdate = true;
}

Vector3 Object3D::rotatePoint(const Vector2 &point, float angle)
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    float radius = fabs(point.x); // Distância do eixo Y (sempre positiva)
    float height = point.y;       // Altura

    float x = radius * cosAngle;
    float y = height;
    float z = radius * sinAngle;

    return Vector3(x, y, z);
}

void Object3D::generateVertices()
{
    if (profile.empty())
        return;

    vertices.clear();
    vertices.resize(rotationSteps);

    float angleStep = (2.0f * MY_PI) / rotationSteps;

    for (int i = 0; i < rotationSteps; i++) {
        float angle = i * angleStep;
        vertices[i].clear();
        vertices[i].reserve(profile.size());

        for (const Vector2 &point : profile) {
            vertices[i].push_back(rotatePoint(point, angle));
        }
    }

    needsUpdate = false;
}

void Object3D::generateTriangles()
{
    triangles.clear();

    if (profile.size() < 2 || rotationSteps < 3)
        return;

    // Para cada seção da rotação
    for (int i = 0; i < rotationSteps; i++) {
        int nextI = (i + 1) % rotationSteps;

        // Para cada segmento ao longo do perfil
        for (size_t j = 0; j < profile.size() - 1; j++) {
            if (j + 1 < vertices[i].size() && j + 1 < vertices[nextI].size()) {
                // Vértices do quad atual
                Vector3 v1 = vertices[i][j];         // (i, j)
                Vector3 v2 = vertices[nextI][j];     // (i+1, j)
                Vector3 v3 = vertices[nextI][j + 1]; // (i+1, j+1)
                Vector3 v4 = vertices[i][j + 1];     // (i, j+1)

                // Divide o quad em dois triângulos
                // Triângulo 1: v1, v2, v3
                triangles.emplace_back(v1, v2, v3);

                // Triângulo 2: v1, v3, v4
                triangles.emplace_back(v1, v3, v4);
            }
        }
    }
}

const std::vector<std::vector<Vector3>> &Object3D::getVertices() const
{
    if (needsUpdate) {
        const_cast<Object3D *>(this)->generateVertices();
        const_cast<Object3D *>(this)->generateTriangles();
    }
    return vertices;
}

const std::vector<Triangle3D> &Object3D::getTriangles() const
{
    if (needsUpdate) {
        const_cast<Object3D *>(this)->generateVertices();
        const_cast<Object3D *>(this)->generateTriangles();
    }
    return triangles;
}
