#ifndef TANK_H
#define TANK_H

#include "Polygon.h"
#include "Projectile.h"
#include "Vector2.h"

class Tank {
private:
  Vector2 position;
  float baseAngle;   // Ângulo da base em radianos
  float turretAngle; // Ângulo da torre em radianos
  float speed = 100.0f;
  float rotationSpeed = 2.0f;

  // Partes do tanque
  Polygon base;   // Corpo principal
  Polygon turret; // Canhão

  // Saúde
  float health = 100.0f;
  const float maxHealth = 100.0f;

  // Controles
  bool rotatingLeft = false;
  bool rotatingRight = false;

  // Disparo
  float fireCooldown = 0.3f;
  float timeSinceLastShot = 0.0f;
  std::vector<Projectile> projectiles;

public:
  Tank(float x, float y) : position(x, y), baseAngle(0), turretAngle(0) {
    // Configura as partes do tanque
    base.createRectangle(50, 30);   // Corpo principal
    turret.createRectangle(40, 10); // Canhão
    std::vector<Projectile> projectiles;
  }

  void update(float deltaTime) {
    // Atualiza rotação
    if (rotatingLeft) {
      baseAngle += rotationSpeed * deltaTime;
    } else if (rotatingRight) {
      baseAngle -= rotationSpeed * deltaTime;
    }

    // Movimentação na direção atual
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

  void render() {
    // Desenha o corpo principal
    CV::color(0.5f, 0.5f, 0.5f); // Cinza
    base.transform(baseAngle, position);
    base.fill();

    // Desenha o canhão (posição ajustada)
    CV::color(4);
    Vector2 turretPos =
        position + Vector2(cos(turretAngle) * 25, sin(turretAngle) * 25);
    turret.transform(turretAngle, turretPos);
    turret.fill();

    // Desenha projéteis
    for (auto &projectile : projectiles) {
      projectile.render();
    }

    // Desenha a barra de saúde
    drawHealthBar();
  }

  void shoot() {
    if (timeSinceLastShot >= fireCooldown) {
      Vector2 spawnPos =
          position + Vector2(cos(turretAngle) * 45, sin(turretAngle) * 45);
      projectiles.emplace_back(spawnPos, getTurretDirection(), 300.0f);
      timeSinceLastShot = 0.0f;
    }
  }

  void drawHealthBar() {
    float barWidth = 60.0f;
    float barHeight = 5.0f;
    float yOffset = -30.0f; // Posição abaixo do tanque

    // Fundo da barra
    CV::color(0.7f, 0.7f, 0.7f);
    CV::rectFill(
        position.x - barWidth / 2, position.y + yOffset - barHeight / 2,
        position.x + barWidth / 2, position.y + yOffset + barHeight / 2);

    // Vida (verde para vermelho conforme diminui)
    float healthRatio = health / maxHealth;
    CV::color(1.0f - healthRatio, healthRatio, 0);
    CV::rectFill(position.x - barWidth / 2,
                 position.y + yOffset - barHeight / 2,
                 position.x - barWidth / 2 + barWidth * healthRatio,
                 position.y + yOffset + barHeight / 2);
  }

  void setTurretAngle(float mouseX, float mouseY) {
    turretAngle = atan2(mouseY - position.y, mouseX - position.x);
  }

  void startRotatingLeft() { rotatingLeft = true; }
  void stopRotatingLeft() { rotatingLeft = false; }
  void startRotatingRight() { rotatingRight = true; }
  void stopRotatingRight() { rotatingRight = false; }

  Vector2 getPosition() const { return position; }
  Vector2 getTurretDirection() const {
    return Vector2(cos(turretAngle), sin(turretAngle));
  }

  void takeDamage(float damage) {
    if (damage <= 0)
      return; // Ignora danos não positivos

    health = std::max(health - damage, 0.0f); // Mais conciso

    if (health == 0) {
      //
    }
  }

  std::vector<Projectile> &getProjectiles() { return projectiles; }

  // Adicionar método para marcar projéteis para destruição
  void markProjectileForDestruction(size_t index) {
    if (index < projectiles.size()) {
      projectiles[index].markForDestruction();
    }
  }
};

#endif
