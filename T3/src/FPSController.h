#ifndef FPS_CONTROLLER_H
#define FPS_CONTROLLER_H

#include <algorithm>
#include <chrono>

class FPSController {
private:
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  TimePoint m_startTime;     // Tempo inicial do frame
  TimePoint m_lastFPSUpdate; // Última atualização do FPS
  float m_deltaTime;         // Tempo entre frames em segundos
  float m_fps;               // Frames por segundo
  int m_frameCount;          // Contador de frames acumulados

  const float MAX_DELTA_TIME =
      0.1f; // Limite para o deltaTime para evitar problemas em baixos FPS

public:
  FPSController()
      : m_deltaTime(0.016f), // Valor inicial de 60 FPS
        m_fps(60.0f), m_frameCount(0) {
    m_startTime = Clock::now();
    m_lastFPSUpdate = m_startTime;
  }

  // Chamar a cada frame
  void update() {
    // Calcula deltaTime
    TimePoint now = Clock::now();
    m_deltaTime = std::chrono::duration<float>(now - m_startTime).count();
    m_startTime = now;

    // Limita o deltaTime para evitar problemas em baixos FPS
    m_deltaTime = std::min(m_deltaTime, MAX_DELTA_TIME);

    // Atualiza FPS a cada 1 segundo
    m_frameCount++;
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - m_lastFPSUpdate)
            .count();

    if (elapsed >= 1) {
      m_fps = static_cast<float>(m_frameCount) / elapsed;
      m_frameCount = 0;
      m_lastFPSUpdate = now;
    }
  }

  // Tempo entre frames em segundos
  float deltaTime() const { return m_deltaTime; }

  // FPS atualizado a cada segundo
  float fps() const { return m_fps; }
};

#endif
