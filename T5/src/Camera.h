/**
 * Camera.h
 *
 * Classe responsável pelo controle da câmera/nave espacial
 * Implementa movimentação baseada em vetores conforme requisitos
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include <GL/glut.h>
#include <cmath>

class Camera
{
  private:
    Vector3 position;  // Posição da câmera no espaço
    Vector3 direction; // Vetor direção normalizado (para onde a câmera aponta)
    Vector3 up;        // Vetor up normalizado
    Vector3 right;     // Vetor right normalizado -> calculado automaticamente

    // Ângulos de rotação (em graus)
    float pitch; // Rotação em X (cima/baixo)
    float yaw;   // Rotação em Y (esquerda/direita)
    float roll;  // Rotação em Z (inclinação)

    void updateVectors();

  public:
    Camera();
    Camera(const Vector3 &pos, const Vector3 &dir, const Vector3 &upVec);

    Vector3 getPosition() const { return position; }
    Vector3 getDirection() const { return direction; }
    Vector3 getUp() const { return up; }
    Vector3 getRight() const { return right; }

    // Movimento baseado em vetores
    void move(const Vector3 &movement);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);

    // Rotação
    void rotate(const Vector3 &rotation);
    void setPitch(float angle);
    void setYaw(float angle);
    void setRoll(float angle);

    // Aplicar transformação OpenGL
    void applyTransform();

    // Reset da câmera
    void reset(const Vector3 &pos, const Vector3 &dir, const Vector3 &upVec);
};

#endif
