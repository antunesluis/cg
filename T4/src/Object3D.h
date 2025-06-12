#ifndef __OBJECT_3D_H__
#define __OBJECT_3D_H__

#include "Vector2.h"
#include "Vector3.h"
#include <vector>

class Object3D
{
  private:
    std::vector<std::vector<Vector3>> vertices; // Matriz de vértices [rotação][ponto_curva]
    std::vector<Vector2> profile;               // Perfil 2D original
    int rotationSteps;                          // Número de passos de rotação
    mutable bool needsUpdate;

    void generateVertices();
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
};

#endif
