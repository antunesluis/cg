#ifndef TANK_H
#define TANK_H

#include "Colors.h"
#include "Projectile.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <vector>

class Tank
{
  private:
    // Propriedades do tanque
    Vector2 position;
    float baseAngle;   // Ângulo da base em radianos
    float turretAngle; // Ângulo da torre em radianos
    float speed = 80.0f;
    float rotationSpeed = 2.0f;

    // Geometria do tanque
    std::vector<Vector2> baseVertices;
    std::vector<Vector2> turretVertices;
    std::vector<Vector2> transformedBaseVertices;
    std::vector<Vector2> transformedTurretVertices;

    // Saúde
    float health = 150.0f;
    const float maxHealth = 100.0f;

    // Controles
    bool rotatingLeft = false;
    bool rotatingRight = false;

    // Disparo
    float fireCooldown = 0.3f;
    float timeSinceLastShot = 0.0f;
    std::vector<Projectile> projectiles;

    // Métodos auxiliares de desenho
    void createBaseRectangle(float width, float height)
    {
        baseVertices.clear();
        float halfW = width / 2;
        float halfH = height / 2;
        baseVertices = {Vector2(-halfW, -halfH), Vector2(-halfW, halfH), Vector2(halfW, halfH), Vector2(halfW, -halfH)};
        transformedBaseVertices = baseVertices;
    }

    void createTurretRectangle(float width, float height)
    {
        turretVertices.clear();
        float halfW = width / 2;
        float halfH = height / 2;
        turretVertices = {Vector2(-halfW, -halfH), Vector2(-halfW, halfH), Vector2(halfW, halfH),
                          Vector2(halfW, -halfH)};
        transformedTurretVertices = turretVertices;
    }

    void transformVertices(std::vector<Vector2> &original, std::vector<Vector2> &transformed, float angle,
                           const Vector2 &pos)
    {
        float cosA = cos(angle);
        float sinA = sin(angle);

        for (size_t i = 0; i < original.size(); i++) {
            float x = original[i].x * cosA - original[i].y * sinA;
            float y = original[i].x * sinA + original[i].y * cosA;
            transformed[i].x = x + pos.x;
            transformed[i].y = y + pos.y;
        }
    }

    void drawPolygon(const std::vector<Vector2> &vertices) const
    {
        if (vertices.size() < 3)
            return;

        std::vector<float> vx, vy;
        for (const auto &v : vertices) {
            vx.push_back(v.x);
            vy.push_back(v.y);
        }
        CV::polygonFill(vx.data(), vy.data(), (int)vx.size());
    }

  public:
    Tank(float x, float y) : position(x, y), baseAngle(0), turretAngle(0)
    {
        createBaseRectangle(50, 30);   // Corpo principal
        createTurretRectangle(40, 10); // Canhão
    }

    void update(float deltaTime)
    {
        // Atualiza rotação
        if (rotatingLeft) {
            baseAngle += rotationSpeed * deltaTime;
        } else if (rotatingRight) {
            baseAngle -= rotationSpeed * deltaTime;
        }

        // Movimentação
        position.x += cos(baseAngle) * speed * deltaTime;
        position.y += sin(baseAngle) * speed * deltaTime;

        // Atualiza cooldown do disparo
        timeSinceLastShot += deltaTime;

        // Atualiza projéteis
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            it->update(deltaTime);
            if (it->shouldDestroy()) {
                it = projectiles.erase(it);
            } else {
                ++it;
            }
        }
    }

    void render()
    {
        // Desenha o corpo principal
        Colors::tankBase();
        transformVertices(baseVertices, transformedBaseVertices, baseAngle, position);
        drawPolygon(transformedBaseVertices);

        // Desenha o canhão
        Colors::tankTurret();
        Vector2 turretPos = position + Vector2(cos(turretAngle) * 25, sin(turretAngle) * 25);
        transformVertices(turretVertices, transformedTurretVertices, turretAngle, turretPos);
        drawPolygon(transformedTurretVertices);

        // Desenha projéteis
        for (auto &projectile : projectiles) {
            projectile.render();
        }

        // Desenha barra de saúde
        drawHealthBar();
    }

    void shoot()
    {
        if (timeSinceLastShot >= fireCooldown) {
            Vector2 spawnPos = position + Vector2(cos(turretAngle) * 45, sin(turretAngle) * 45);
            projectiles.emplace_back(spawnPos, getTurretDirection(), 300.0f);
            timeSinceLastShot = 0.0f;
        }
    }

    void drawHealthBar()
    {
        float barWidth = 60.0f;
        float barHeight = 5.0f;
        float yOffset = -30.0f; // Posição abaixo do tanque

        // Fundo da barra
        CV::color(0.7f, 0.7f, 0.7f);
        CV::rectFill(position.x - barWidth / 2, position.y + yOffset - barHeight / 2, position.x + barWidth / 2,
                     position.y + yOffset + barHeight / 2);

        // Vida (verde para vermelho conforme diminui)
        float healthRatio = health / maxHealth;
        CV::color(1.0f - healthRatio, healthRatio, 0);
        CV::rectFill(position.x - barWidth / 2, position.y + yOffset - barHeight / 2,
                     position.x - barWidth / 2 + barWidth * healthRatio, position.y + yOffset + barHeight / 2);
    }

    void setTurretAngle(float mouseX, float mouseY) { turretAngle = atan2(mouseY - position.y, mouseX - position.x); }

    void startRotatingLeft() { rotatingLeft = true; }
    void stopRotatingLeft() { rotatingLeft = false; }
    void startRotatingRight() { rotatingRight = true; }
    void stopRotatingRight() { rotatingRight = false; }

    Vector2 getPosition() const { return position; }
    Vector2 getTurretDirection() const { return Vector2(cos(turretAngle), sin(turretAngle)); }

    void takeDamage(float damage)
    {
        if (damage <= 0)
            return; // Ignora danos não positivos

        health = std::max(health - damage, 0.0f); // Mais conciso

        if (health == 0) {
            //
        }
    }

    std::vector<Projectile> &getProjectiles() { return projectiles; }

    // Adicionar método para marcar projéteis para destruição
    void markProjectileForDestruction(size_t index)
    {
        if (index < projectiles.size()) {
            projectiles[index].markForDestruction();
        }
    }

    Vector2 &getPositionRef() { return position; }

    // Método para aplicar força de repulsão
    void applyPush(const Vector2 &direction, float force) { position += direction * force; }

    float getHealth() const { return health; }
};

#endif
