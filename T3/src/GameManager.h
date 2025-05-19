#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BezierTrack.h"
#include "Collision.h"
#include "FPSController.h"
#include "KeyCodes.h"
#include "Tank.h"
#include "Target.h"
#include "gl_canvas2d.h"
#include <string>
#include <vector>

class GameManager {
private:
  FPSController fpsCtrl;

  // Game state
  bool isEditorMode = false;
  int score = 0;

  // Game objects
  Tank *playerTank = nullptr;
  std::vector<Target> targets;
  BezierTrack track;
  int screenWidth = 1920, screenHeight = 1080;

  // Input state
  bool keyStates[256] = {false};
  int mouseX = 0, mouseY = 0;

  void initGame() {
    playerTank = new Tank(screenWidth / 2, screenHeight / 2);

    // Criar uma pista padrão se nenhuma for carregada
    if (!track.isValid()) {
      createDefaultTrack();
    }

    generateRandomTargets(10);
  }

  void createDefaultTrack() {
    track.clear();

    // Criar uma pista circular simples
    float centerX = screenWidth / 2;
    float centerY = screenHeight / 2;
    float radius = 350;

    // Pontos para uma pista circular
    track.addControlPoint(centerX + radius, centerY);
    track.addControlPoint(centerX + radius, centerY - radius);
    track.addControlPoint(centerX, centerY - radius);
    track.addControlPoint(centerX - radius, centerY);
    track.addControlPoint(centerX - radius, centerY + radius);
    track.addControlPoint(centerX, centerY + radius);
    track.addControlPoint(centerX + radius, centerY);
    track.addControlPoint(centerX + radius, centerY - radius);
    track.addControlPoint(centerX, centerY - radius);
    track.addControlPoint(centerX - radius, centerY);

    track.setTrackWidth(150.0f);
  }

  void generateRandomTargets(int count) {
    targets.clear();

    if (track.isValid()) {
      // Obter pontos na pista para posicionar os alvos
      const auto &trackPoints = track.getTrackPoints();
      if (!trackPoints.empty()) {
        for (int i = 0; i < count; i++) {
          // Escolher pontos aleatórios na pista
          int index = rand() % trackPoints.size();
          targets.emplace_back(trackPoints[index].x, trackPoints[index].y);
        }
        return;
      }
    }

    // Fallback para posicionamento aleatório se a pista não estiver pronta
    for (int i = 0; i < count; i++) {
      float x = 100 + rand() % (screenWidth - 200);
      float y = 100 + rand() % (screenHeight - 200);
      targets.emplace_back(x, y);
    }
  }

  void checkCollisions() {
    auto &projectiles = playerTank->getProjectiles();

    // Colisão projéteis-alvos
    for (size_t i = 0; i < projectiles.size(); i++) {
      for (size_t j = 0; j < targets.size(); j++) {
        if (!targets[j].isDestroyed() &&
            Collision::circleCircle(
                projectiles[i].getPosition(), projectiles[i].getRadius(),
                targets[j].getPosition(), targets[j].getRadius())) {

          targets[j].takeDamage(25);
          playerTank->markProjectileForDestruction(i);

          if (targets[j].isDestroyed()) {
            score += 10;
          }
        }
      }
    }

    // Colisão tanque-alvos
    for (auto &target : targets) {
      if (!target.isDestroyed() &&
          Collision::rectCircle(playerTank->getPosition(), 50, 30,
                                target.getPosition(), target.getRadius())) {
        playerTank->takeDamage(2);
        target.takeDamage(5);
      }
    }

    // Colisão do tanque com a pista
    if (track.isValid()) {
      if (track.checkTankCollision(playerTank->getPosition(), 25.0f)) {
        playerTank->takeDamage(1);
      }
    }

    // Colisão dos projéteis com a pista
    // (Implementação opcional - deixei comentada para simplificar)
    /*
    for (size_t i = 0; i < projectiles.size(); i++) {
      if (track.isValid() && !track.isInsideTrack(projectiles[i].getPosition()))
    { playerTank->markProjectileForDestruction(i);
      }
    }
    */
  }

  void resetGame() {
    score = 0;
    if (playerTank) {
      playerTank->getPosition().set(screenWidth / 2, screenHeight / 2);
      playerTank->takeDamage(-100); // Restaura a vida
    }
    generateRandomTargets(10);
  }

public:
  GameManager() { initGame(); }

  ~GameManager() {
    if (playerTank)
      delete playerTank;
  }

  void update() {
    fpsCtrl.update();
    float dt = fpsCtrl.deltaTime();

    if (!isEditorMode) {
      playerTank->update(dt);
      checkCollisions();

      // Remove alvos destruídos
      targets.erase(
          std::remove_if(targets.begin(), targets.end(),
                         [](const Target &t) { return t.isDestroyed(); }),
          targets.end());

      if (targets.empty()) {
        resetGame();
      }
    }
  }

  void render() {
    CV::clear(0.2f, 0.2f, 0.2f);

    // Renderizar a pista
    track.render();

    // Render targets
    for (auto &target : targets) {
      target.render();
    }

    if (playerTank && !isEditorMode) {
      playerTank->render();
    }

    CV::color(1, 1, 1);
    CV::text(10, 10, ("FPS: " + std::to_string((int)fpsCtrl.fps())).c_str());
    CV::text(10, 30, ("Score: " + std::to_string(score)).c_str());

    if (isEditorMode) {
      CV::text(10, 50, "EDITOR MODE - Click to add control points");
    }
  }

  void keyboard(int key) {
    keyStates[key] = true;

    switch (key) {
    case Key::Letter::D:
      playerTank->startRotatingLeft();
      break;

    case Key::Letter::A:
      playerTank->startRotatingRight();
      break;
    }

    // Toggle editor mode with 'E'
    if (key == 'e' || key == 'E') {
      isEditorMode = !isEditorMode;
      track.setEditMode(isEditorMode);
    }

    // Remover último ponto de controle com 'Z' no modo editor
    if (isEditorMode && (key == 'z' || key == 'Z')) {
      track.removeLastControlPoint();
    }

    // Limpar pista com 'C' no modo editor
    if (isEditorMode && (key == 'c' || key == 'C')) {
      track.clear();
    }
  }

  void keyboardUp(int key) {
    keyStates[key] = false;

    switch (key) {
    case Key::Letter::D:
      playerTank->stopRotatingLeft();
      break;

    case Key::Letter::A:
      playerTank->stopRotatingRight();
      break;
    }
  }

  void mouse(int button, int state, int wheel, int direction, int x, int y) {
    mouseX = x;
    mouseY = y;

    playerTank->setTurretAngle(x, y);

    // Adicionar pontos de controle no modo editor
    if (isEditorMode && button == 0 &&
        state == 0) { // Left mouse button pressed
      track.addControlPoint(x, y);
    }
    // Atirar fora do modo editor
    else if (!isEditorMode && button == 0 && state == 0) {
      playerTank->shoot();
    }
  }

  // Getters for keyboard and mouse state
  bool isKeyPressed(int key) const { return keyStates[key]; }
  int getMouseX() const { return mouseX; }
  int getMouseY() const { return mouseY; }
};

#endif
