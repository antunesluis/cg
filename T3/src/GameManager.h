#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "BezierTrack.h"
#include "Collision.h"
#include "Colors.h"
#include "FPSController.h"
#include "KeyCodes.h"
#include "ParticleSystem.h"
#include "Tank.h"
#include "Target.h"
#include "gl_canvas2d.h"
#include <string>
#include <vector>

class GameManager
{
  private:
    enum class GameState
    {
        MAIN_MENU,
        PLAYING,
        EDITOR,
        GAME_OVER
    };

    GameState currentState = GameState::MAIN_MENU;
    FPSController fpsCtrl;
    ParticleSystem particleSystem;

    // Game state
    bool isEditorMode = false;
    int score = 0;
    int highScore = 0;

    // Game objects
    Tank *playerTank = nullptr;
    std::vector<Target> targets;
    BezierTrack track;
    int screenWidth = 1920, screenHeight = 1080;

    // Input state
    bool keyStates[256] = {false};
    int mouseX = 0, mouseY = 0;

    // Wave control
    int currentWave = 1;
    float difficultyMultiplier = 1.0f;
    const float DIFFICULTY_INCREASE_RATE = 0.2f; // 20% mais difícil a cada onda

    void initGame()
    {
        if (!track.isValid()) {
            createDefaultTrack();
        }

        Vector2 startPos = track.getRandomStartPosition();
        playerTank = new Tank(startPos.x, startPos.y);
        generateRandomTargets(10);
    }

    void renderMainMenu()
    {
        Colors::backgroundDefault();

        Colors::uiText();
        CV::text(screenWidth / 2 - 100, screenHeight / 2 - 100, "TANK GAME - MAIN MENU");

        Colors::uiText();
        CV::text(screenWidth / 2 - 50, screenHeight / 2, "1 - Start Default Track");
        CV::text(screenWidth / 2 - 50, screenHeight / 2 + 50, "2 - Track Editor");

        CV::color(0.8, 0.8, 0.8);
        Colors::uiTextHighlight();
        CV::text(screenWidth / 2 - 100, screenHeight / 2 + 150, "Press ESC to quit");

        // Mostra high score
        Colors::uiTextHighlight();
        CV::text(screenWidth / 2 - 80, screenHeight / 2 + 200, ("High Score: " + std::to_string(highScore)).c_str());
    }

    void renderGameOver()
    {
        Colors::backgroundDefault();

        Colors::red();
        CV::text(screenWidth / 2 - 100, screenHeight / 2 - 50, "GAME OVER");

        Colors::uiText();
        CV::text(screenWidth / 2 - 80, screenHeight / 2 + 20, ("Score: " + std::to_string(score)).c_str());
        CV::text(screenWidth / 2 - 100, screenHeight / 2 + 70, ("High Score: " + std::to_string(highScore)).c_str());

        CV::text(screenWidth / 2 - 100, screenHeight / 2 + 120, "Press R to Restart");
        CV::text(screenWidth / 2 - 100, screenHeight / 2 + 170, "Press M for Main Menu");
    }

    void createDefaultTrack()
    {
        track.clear();
        float centerX = screenWidth / 2;
        float centerY = screenHeight / 2;
        float radius = 380;
        float controlOffset = radius * 0.6f; // Distância dos pontos de controle

        // Ponto 1 (leste)
        track.addControlPoint(centerX + radius, centerY);
        track.addControlPoint(centerX + radius, centerY - controlOffset);
        track.addControlPoint(centerX + controlOffset, centerY - radius);

        // Ponto 2 (norte)
        track.addControlPoint(centerX, centerY - radius);
        track.addControlPoint(centerX - controlOffset, centerY - radius);
        track.addControlPoint(centerX - radius, centerY - controlOffset);

        // Ponto 3 (oeste)
        track.addControlPoint(centerX - radius, centerY);
        track.addControlPoint(centerX - radius, centerY + controlOffset);
        track.addControlPoint(centerX - controlOffset, centerY + radius);

        // Ponto 4 (sul)
        track.addControlPoint(centerX, centerY + radius);
        track.addControlPoint(centerX + controlOffset, centerY + radius);
        track.addControlPoint(centerX + radius, centerY + controlOffset);

        track.setTrackWidth(200.0f);
    }

    // GameManager.h
    void generateRandomTargets(int count)
    {
        targets.clear();

        if (track.isValid()) {
            const auto &trackPoints = track.getTrackPoints();
            if (!trackPoints.empty()) {
                // Distribui os alvos ao longo da pista, evitando posições muito
                // próximas
                float minDistanceBetweenTargets = 100.0f; // Distância mínima entre alvos
                std::vector<int> usedIndices;

                for (int i = 0; i < count; i++) {
                    bool positionValid = false;
                    int attempts = 0;
                    int maxAttempts = 50;
                    Vector2 position;

                    // Tenta encontrar uma posição válida
                    while (!positionValid && attempts < maxAttempts) {
                        int index = rand() % trackPoints.size();
                        position = trackPoints[index];

                        // Verifica se está longe o suficiente de outros alvos
                        positionValid = true;
                        for (const auto &target : targets) {
                            if ((position - target.getPosition()).length() < minDistanceBetweenTargets) {
                                positionValid = false;
                                break;
                            }
                        }

                        // Verifica se está longe do jogador inicial
                        if (playerTank && (position - playerTank->getPosition()).length() < 200.0f) {
                            positionValid = false;
                        }

                        attempts++;
                    }

                    if (positionValid) {
                        targets.emplace_back(position.x, position.y);
                        Target &newTarget = targets.back();

                        // Aplica o multiplicador de dificuldade
                        newTarget.setSpeed(20.0f + rand() % 30 * difficultyMultiplier); // Velocidade aumentada
                        newTarget.setHealth(70 + rand() % 60 * difficultyMultiplier);   // Vida aumentada
                    }
                }
                return;
            }
        }

        // Fallback: gera alvos em posições aleatórias se não houver pista válida
        for (int i = 0; i < count; i++) {
            float x = 100 + rand() % (screenWidth - 200);
            float y = 100 + rand() % (screenHeight - 200);
            targets.emplace_back(x, y);
        }
    }

    void checkCollisions()
    {
        auto &projectiles = playerTank->getProjectiles();

        // 1. Colisão projéteis com a pista e alvos
        for (size_t i = 0; i < projectiles.size(); i++) {
            // Colisão com a pista
            auto trackCollision = track.checkCollision(projectiles[i].getPosition(), projectiles[i].getRadius());
            if (trackCollision.collided) {
                particleSystem.createExplosion(projectiles[i].getPosition(),
                                               10); // Pequena explosão
                playerTank->markProjectileForDestruction(i);
                continue;
            }

            // Colisão com alvos
            for (size_t j = 0; j < targets.size(); j++) {
                if (!targets[j].isDestroyed() &&
                    Collision::circleCircle(projectiles[i].getPosition(), projectiles[i].getRadius(),
                                            targets[j].getPosition(), targets[j].getRadius())) {
                    targets[j].takeDamage(25);
                    particleSystem.createExplosion(targets[j].getPosition(),
                                                   30); // Explosão grande

                    if (targets[j].isDestroyed()) {
                        score += 10;
                        highScore = std::max(score, highScore);
                    }

                    playerTank->markProjectileForDestruction(i);
                    break;
                }
            }
        }

        // 2. Colisão tanque com alvos
        for (auto &target : targets) {
            if (!target.isDestroyed() && Collision::rectCircle(playerTank->getPosition(), 50,
                                                               30, // Largura/altura do tanque
                                                               target.getPosition(), target.getRadius())) {
                playerTank->takeDamage(2);
                target.takeDamage(5);
                particleSystem.createExplosion(target.getPosition(),
                                               15); // Explosão média
            }
        }

        // 3. Colisão tanque com a pista
        auto tankCollision = track.checkCollision(playerTank->getPosition(), 25.0f);
        if (tankCollision.collided) {
            playerTank->takeDamage(1);
            Vector2 pushVector = tankCollision.normal * (tankCollision.penetrationDepth + 1.0f);
            playerTank->applyPush(pushVector, 1.0f);
            particleSystem.createExplosion(playerTank->getPosition(),
                                           5); // Pequeno efeito de impacto
        }

        // Remove projéteis e alvos destruídos
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                         [](const Projectile &p) { return p.shouldDestroy(); }),
                          projectiles.end());

        targets.erase(std::remove_if(targets.begin(), targets.end(), [](const Target &t) { return t.isDestroyed(); }),
                      targets.end());
    }

    void checkGameOver()
    {
        if (playerTank && playerTank->getHealth() <= 0) {
            currentState = GameState::GAME_OVER;
        }
    }

    void restartGame()
    {
        score = 0;
        if (playerTank) {
            delete playerTank;
            playerTank = nullptr;
        }
        targets.clear();
        initGame();
        currentState = GameState::PLAYING;
    }

    void returnToMainMenu()
    {
        if (playerTank) {
            delete playerTank;
            playerTank = nullptr;
        }
        targets.clear();
        track.setEditMode(false);
        isEditorMode = false;
        currentState = GameState::MAIN_MENU;
    }

  public:
    GameManager() { currentState = GameState::MAIN_MENU; }

    ~GameManager()
    {
        if (playerTank)
            delete playerTank;
    }

    void update()
    {
        fpsCtrl.update();
        float dt = fpsCtrl.deltaTime();

        if (currentState != GameState::PLAYING)
            return;

        if (targets.empty()) {
            currentWave++;
            difficultyMultiplier = 1.0f + (currentWave - 1) * DIFFICULTY_INCREASE_RATE;
            generateRandomTargets(10 + currentWave * 2); // Mais alvos a cada onda
            printf("Wave %d started! Difficulty: %.2f", currentWave, difficultyMultiplier);
        }

        playerTank->update(dt);

        particleSystem.update(fpsCtrl.deltaTime()); // Atualiza partículas
        for (auto &target : targets) {
            target.update(fpsCtrl.deltaTime(), track.getTrackPoints()); // Move alvos
        }

        auto collision = track.checkCollision(playerTank->getPosition(), 25.0f);
        if (collision.collided) {
            playerTank->takeDamage(1);
            Vector2 pushVector = collision.normal * (collision.penetrationDepth + 1.0f);
            playerTank->applyPush(pushVector, 1.0f);
        }

        checkCollisions();
        checkGameOver();

        targets.erase(std::remove_if(targets.begin(), targets.end(), [](const Target &t) { return t.isDestroyed(); }),
                      targets.end());
    }

    void render()
    {
        if (currentState == GameState::MAIN_MENU) {
            renderMainMenu();
            return;
        }

        if (currentState == GameState::GAME_OVER) {
            renderGameOver();
            return;
        }

        // Renderização do jogo
        Colors::backgroundDefault();
        track.render();
        particleSystem.render();

        for (auto &target : targets) {
            target.render();
        }

        if (playerTank && !isEditorMode) {
            playerTank->render();
        }

        // UI Principal (FPS/Score/Wave)
        Colors::uiText();
        CV::text(10, 10, ("FPS: " + std::to_string((int)fpsCtrl.fps())).c_str());
        CV::text(10, 30, ("Score: " + std::to_string(score)).c_str());
        CV::text(10, 50, ("Wave: " + std::to_string(currentWave)).c_str());

        // UI do Modo Editor
        if (isEditorMode) {
            CV::text(10, 80, "=== EDITOR MODE ===");
            CV::text(10, 100, "Click: Add control point");
            CV::text(10, 120, "Z: Remove last point");
            CV::text(10, 140, "C: Clear track");
            CV::text(10, 160, "E: Save and exit");
            CV::text(10, 180, "M: Exit without saving");
        }

        // UI Global
        if (currentState != GameState::MAIN_MENU) {
            Colors::uiText();
            CV::text(10, screenHeight - 30, "Press M to return to Main Menu");
        }
    }

    void keyboard(int key)
    {
        if (key == 27) { // ESC
            exit(0);
        }

        if (currentState == GameState::MAIN_MENU) {
            if (key == '1') {
                currentState = GameState::PLAYING;
                createDefaultTrack();
                initGame();
            } else if (key == '2') {
                currentState = GameState::EDITOR;
                track.clear();
                track.setEditMode(true);
                isEditorMode = true;
            }
            return;
        }

        if (currentState == GameState::GAME_OVER) {
            if (key == 'r' || key == 'R') {
                restartGame();
            } else if (key == 'm' || key == 'M') {
                returnToMainMenu();
            }
            return;
        }

        if ((key == 'm' || key == 'M') && currentState != GameState::MAIN_MENU) {
            returnToMainMenu();
            return;
        }

        if (currentState == GameState::PLAYING && playerTank) {
            switch (key) {
            case 'd':
            case 'D':
                playerTank->startRotatingLeft();
                break;
            case 'a':
            case 'A':
                playerTank->startRotatingRight();
                break;
            }
        }

        if (currentState == GameState::EDITOR) {
            switch (key) {
            case 'e':
            case 'E':
                currentState = GameState::PLAYING;
                track.setEditMode(false);
                if (!playerTank) {
                    isEditorMode = false;
                    initGame();
                }
                break;
            case 'z':
            case 'Z':
                track.removeLastControlPoint();
                break;
            case 'c':
            case 'C':
                track.clear();
                break;
            }
        }
    }

    void keyboardUp(int key)
    {
        keyStates[key] = false;

        switch (key) {
        case 'd':
        case 'D':
            if (playerTank)
                playerTank->stopRotatingLeft();
            break;
        case 'a':
        case 'A':
            if (playerTank)
                playerTank->stopRotatingRight();
            break;
        }
    }

    void mouse(int button, int state, int wheel, int direction, int x, int y)
    {
        mouseX = x;
        mouseY = y;

        if (playerTank) {
            playerTank->setTurretAngle(x, y);
        }

        if (isEditorMode && button == 0 && state == 0) {
            track.addControlPoint(x, y);
        } else if (!isEditorMode && button == 0 && state == 0 && playerTank) {
            playerTank->shoot();
        }
    }

    bool isKeyPressed(int key) const { return keyStates[key]; }
    int getMouseX() const { return mouseX; }
    int getMouseY() const { return mouseY; }
};

#endif
