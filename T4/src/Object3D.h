#ifndef __OBJECT_3D_H__
#define __OBJECT_3D_H__

#include "Vector2.h"
#include "Vector3.h"
#include <vector>

struct Triangle3D
{
    Vector3 v1, v2, v3;
    Vector3 normal;

    Triangle3D(const Vector3 &a, const Vector3 &b, const Vector3 &c) : v1(a), v2(b), v3(c) { calculateNormal(); }

    void calculateNormal()
    {
        Vector3 edge1 = v2 - v1;
        Vector3 edge2 = v3 - v1;

        // Produto vetorial para calcular normal
        normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
        normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
        normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
        normal.normalize();
    }
};

class Object3D
{
  private:
    std::vector<std::vector<Vector3>> vertices; // Matriz de vértices [rotação][ponto_curva]
    std::vector<Triangle3D> triangles;          // Lista de triângulos da superfície
    std::vector<Vector2> profile;               // Perfil 2D original
    int rotationSteps;                          // Número de passos de rotação
    mutable bool needsUpdate;

    void generateVertices();
    void generateTriangles(); // Nova função para gerar triângulos
    Vector3 rotatePoint(const Vector2 &point, float angle);

  public:
    Object3D(int steps = 36);

    void setProfile(const std::vector<Vector2> &curvePoints);
    void updateProfile(const std::vector<Vector2> &curvePoints);
    void setRotationSteps(int steps);

    bool isEmpty() const { return profile.empty(); }
    int getRotationSteps() const { return rotationSteps; }
    size_t getProfileSize() const { return profile.size(); }

    const std::vector<std::vector<Vector3>> &getVertices() const;
    const std::vector<Triangle3D> &getTriangles() const; // Nova função
};

#endif
