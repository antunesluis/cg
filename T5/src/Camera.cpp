/**
 * Camera.cpp
 *
 * Implementação da classe Camera
 * Utiliza vetores para movimentação conforme requisitos do trabalho
 */

#include "Camera.h"

Camera::Camera() : position(0, 0, 0), direction(0, 0, -1), up(0, 1, 0), pitch(0), yaw(0), roll(0) { updateVectors(); }

Camera::Camera(const Vector3 &pos, const Vector3 &dir, const Vector3 &upVec)
    : position(pos), direction(dir.normalized()), up(upVec.normalized()), pitch(0), yaw(0), roll(0)
{
    updateVectors();
}

void Camera::updateVectors()
{
    // Normalizar vetores principais
    direction.normalize();
    up.normalize();

    // Calcular vetor right (produto vetorial de direction x up)
    right = direction.cross(up);
    right.normalize();

    // Recalcular up (garantir ortogonalidade)
    up = right.cross(direction);
    up.normalize();
}

void Camera::move(const Vector3 &movement)
{
    // Movimento baseado nos vetores locais da câmera (requisito obrigatório)
    Vector3 deltaPos = right * movement.x +    // Movimento lateral
                       up * movement.y +       // Movimento vertical
                       direction * movement.z; // Movimento frente/trás

    position += deltaPos;
}

void Camera::moveForward(float distance) { position += direction * distance; }

void Camera::moveRight(float distance) { position += right * distance; }

void Camera::moveUp(float distance) { position += up * distance; }

void Camera::rotate(const Vector3 &rotation)
{
    pitch += rotation.x;
    yaw += rotation.y;
    roll += rotation.z;

    // Limitar pitch para evitar gimbal lock
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Manter yaw no range [0, 360)
    while (yaw >= 360.0f)
        yaw -= 360.0f;
    while (yaw < 0.0f)
        yaw += 360.0f;

    // Manter roll no range [-180, 180]
    if (roll > 180.0f)
        roll -= 360.0f;
    if (roll < -180.0f)
        roll += 360.0f;

    // Converter ângulos para radianos
    float pitchRad = pitch * M_PI / 180.0f;
    float yawRad = yaw * M_PI / 180.0f;
    float rollRad = roll * M_PI / 180.0f;

    // Calcular nova direção baseada nos ângulos de Euler
    direction.x = cos(pitchRad) * sin(yawRad);
    direction.y = sin(pitchRad);
    direction.z = cos(pitchRad) * cos(yawRad);
    direction.normalize();

    // Calcular novo vetor up considerando roll
    Vector3 worldUp(0, 1, 0);
    right = direction.cross(worldUp);
    right.normalize();

    Vector3 tempUp = right.cross(direction);
    tempUp.normalize();

    // Aplicar roll
    up = tempUp * cos(rollRad) + right * sin(rollRad);
    up.normalize();

    updateVectors();
}

void Camera::setPitch(float angle)
{
    pitch = angle;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    float pitchRad = pitch * M_PI / 180.0f;
    float yawRad = yaw * M_PI / 180.0f;

    direction.x = cos(pitchRad) * sin(yawRad);
    direction.y = sin(pitchRad);
    direction.z = cos(pitchRad) * cos(yawRad);

    updateVectors();
}

void Camera::setYaw(float angle)
{
    yaw = angle;
    while (yaw >= 360.0f)
        yaw -= 360.0f;
    while (yaw < 0.0f)
        yaw += 360.0f;

    float pitchRad = pitch * M_PI / 180.0f;
    float yawRad = yaw * M_PI / 180.0f;

    direction.x = cos(pitchRad) * sin(yawRad);
    direction.y = sin(pitchRad);
    direction.z = cos(pitchRad) * cos(yawRad);

    updateVectors();
}

void Camera::setRoll(float angle)
{
    roll = angle;
    if (roll > 180.0f)
        roll -= 360.0f;
    if (roll < -180.0f)
        roll += 360.0f;

    float rollRad = roll * M_PI / 180.0f;
    Vector3 worldUp(0, 1, 0);
    right = direction.cross(worldUp);
    right.normalize();

    Vector3 tempUp = right.cross(direction);
    tempUp.normalize();

    up = tempUp * cos(rollRad) + right * sin(rollRad);
    up.normalize();

    updateVectors();
}

void Camera::applyTransform()
{
    Vector3 target = position + direction;
    gluLookAt(position.x, position.y, position.z, // Posição da câmera
              target.x, target.y, target.z,       // Ponto de mira
              up.x, up.y, up.z);                  // Vetor up
}

void Camera::reset(const Vector3 &pos, const Vector3 &dir, const Vector3 &upVec)
{
    position = pos;
    direction = dir.normalized();
    up = upVec.normalized();
    pitch = yaw = roll = 0;
    updateVectors();
}
