#include "Object3D.h"
#include "gl_canvas2d.h"
#include <math.h>

Object3D::Object3D(int steps) : rotationSteps(steps), needsUpdate(true) { vertices.clear(); }

void Object3D::setProfile(const std::vector<Vector2> &curvePoints)
{
    profile = curvePoints;
    needsUpdate = true;
    generateVertices(); // Força geração imediata
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
    // Rotação em torno do eixo Y (vertical)
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    // O ponto 2D representa (distância_do_eixo, altura)
    // Convertemos para 3D: (x, y, z) onde y é altura e x,z formam o círculo
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

    float angleStep = (2.0f * M_PI) / rotationSteps;

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

const std::vector<std::vector<Vector3>> &Object3D::getVertices() const
{
    if (needsUpdate) {
        const_cast<Object3D *>(this)->generateVertices();
    }
    return vertices;
}
