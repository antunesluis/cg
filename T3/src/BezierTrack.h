#ifndef BEZIER_TRACK_H
#define BEZIER_TRACK_H

#include "Vector2.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <vector>

class BezierTrack {
private:
  // Pontos de controle para a curva de Bézier
  std::vector<Vector2> controlPoints;

  // Pontos calculados da curva (para renderização)
  std::vector<Vector2> trackPoints;

  // Pontos que definem os limites da pista
  std::vector<Vector2> innerBoundary;
  std::vector<Vector2> outerBoundary;

  // Largura da pista
  float trackWidth = 100.0f;

  // Número de segmentos para cálculo da curva por seção de Bézier
  const int numSegmentsPerSection = 20;

  // Flag para modo de edição
  bool editMode = false;

  // Calcular um ponto na curva de Bézier cúbica
  Vector2 calculateBezierPoint(float t, const Vector2 &p0, const Vector2 &p1,
                               const Vector2 &p2, const Vector2 &p3) const {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    // Fórmula da curva de Bézier cúbica
    Vector2 p = p0 * uuu;   // (1-t)³ * P0
    p += p1 * (3 * uu * t); // 3(1-t)² * t * P1
    p += p2 * (3 * u * tt); // 3(1-t) * t² * P2
    p += p3 * ttt;          // t³ * P3

    return p;
  }

  // Calcula a tangente da curva em um ponto específico (derivada da função de
  // Bézier)
  Vector2 calculateTangent(float t, const Vector2 &p0, const Vector2 &p1,
                           const Vector2 &p2, const Vector2 &p3) const {
    float u = 1.0f - t;
    float uu = u * u;
    float tt = t * t;

    // Derivada da curva de Bézier cúbica
    Vector2 tangent = p0 * (-3 * uu);
    tangent += p1 * (3 * uu - 6 * u * t);
    tangent += p2 * (6 * u * t - 3 * tt);
    tangent += p3 * (3 * tt);

    return tangent;
  }

  // Calcula a normal à curva usando a tangente
  Vector2 calculateNormal(const Vector2 &tangent) const {
    Vector2 normal = tangent;
    normal.normalize();
    // Rotação de 90 graus no sentido horário (perpendicular à tangente)
    return Vector2(-normal.y, normal.x);
  }

  // Recalcula todos os pontos da pista a partir dos pontos de controle
  void recalculateTrack() {
    // Precisamos de pelo menos 4 pontos para formar uma curva de Bézier cúbica
    if (controlPoints.size() < 4)
      return;

    trackPoints.clear();
    innerBoundary.clear();
    outerBoundary.clear();

    // Número total de curvas de Bézier que podemos formar
    int numCurves = (controlPoints.size() - 1) / 3;

    // Para cada curva de Bézier cúbica
    for (int curve = 0; curve < numCurves; curve++) {
      int baseIndex = curve * 3;

      // Garantir que temos pontos suficientes
      if (baseIndex + 3 >= controlPoints.size())
        break;

      Vector2 p0 = controlPoints[baseIndex];
      Vector2 p1 = controlPoints[baseIndex + 1];
      Vector2 p2 = controlPoints[baseIndex + 2];
      Vector2 p3 = controlPoints[baseIndex + 3];

      // Calcular os pontos da curva de Bézier e suas tangentes
      for (int seg = 0; seg <= numSegmentsPerSection; seg++) {
        float t = static_cast<float>(seg) / numSegmentsPerSection;
        Vector2 point = calculateBezierPoint(t, p0, p1, p2, p3);
        Vector2 tangent = calculateTangent(t, p0, p1, p2, p3);
        Vector2 normal = calculateNormal(tangent);

        // Adiciona o ponto central da pista
        trackPoints.push_back(point);

        // Calcula e adiciona os pontos das bordas
        innerBoundary.push_back(point + normal * (trackWidth / 2));
        outerBoundary.push_back(point - normal * (trackWidth / 2));
      }
    }
  }

public:
  BezierTrack() {}

  // Adiciona um ponto de controle à pista
  void addControlPoint(float x, float y) {
    controlPoints.emplace_back(x, y);
    recalculateTrack();
  }

  // Remove o último ponto de controle
  void removeLastControlPoint() {
    if (!controlPoints.empty()) {
      controlPoints.pop_back();
      recalculateTrack();
    }
  }

  // Verifica se um ponto está dentro da pista
  bool isInsideTrack(const Vector2 &point) const {
    if (trackPoints.empty() || innerBoundary.empty() || outerBoundary.empty())
      return true;

    // Algoritmo de ray casting para verificar se um ponto está dentro de um
    // polígono Criamos um polígono com os limites internos e externos

    // Criar um vetor combinado dos limites
    std::vector<Vector2> boundaries;
    for (const auto &p : innerBoundary)
      boundaries.push_back(p);

    // Adicionamos a borda externa em ordem inversa para formar um polígono
    // fechado
    for (int i = outerBoundary.size() - 1; i >= 0; i--)
      boundaries.push_back(outerBoundary[i]);

    if (boundaries.empty())
      return true;

    bool inside = false;
    for (size_t i = 0, j = boundaries.size() - 1; i < boundaries.size();
         j = i++) {
      if (((boundaries[i].y > point.y) != (boundaries[j].y > point.y)) &&
          (point.x < (boundaries[j].x - boundaries[i].x) *
                             (point.y - boundaries[i].y) /
                             (boundaries[j].y - boundaries[i].y) +
                         boundaries[i].x)) {
        inside = !inside;
      }
    }

    return inside;
  }

  // Verifica colisão entre o tanque e os limites da pista
  bool checkTankCollision(const Vector2 &tankPosition, float tankRadius) const {
    // Verificação simplificada - considera o tanque como um círculo
    if (innerBoundary.empty() || outerBoundary.empty())
      return false;

    // Verifica colisão com o limite interno
    for (size_t i = 0; i < innerBoundary.size() - 1; i++) {
      if (distanceToLineSegment(tankPosition, innerBoundary[i],
                                innerBoundary[i + 1]) < tankRadius) {
        return true;
      }
    }

    // Verifica colisão com o limite externo
    for (size_t i = 0; i < outerBoundary.size() - 1; i++) {
      if (distanceToLineSegment(tankPosition, outerBoundary[i],
                                outerBoundary[i + 1]) < tankRadius) {
        return true;
      }
    }

    return false;
  }

  // Calcula a distância de um ponto a um segmento de linha
  float distanceToLineSegment(const Vector2 &point, const Vector2 &lineStart,
                              const Vector2 &lineEnd) const {
    Vector2 line = lineEnd - lineStart;
    float lineLength = line.length();

    if (lineLength == 0.0f)
      return (point - lineStart).length();

    // Projeção do ponto no segmento de linha
    float t = ((point - lineStart) * line) / (lineLength * lineLength);
    t = std::max(0.0f, std::min(1.0f, t));

    Vector2 projection = lineStart + line * t;
    return (point - projection).length();
  }

  // Renderiza a pista
  void render() {
    // Renderizar pontos de controle (apenas no modo de edição)
    if (editMode) {
      // Conectar os pontos de controle com linhas
      if (controlPoints.size() >= 2) {
        CV::color(0.5, 0.5, 0.5); // Cinza
        for (size_t i = 0; i < controlPoints.size() - 1; i++) {
          CV::line(controlPoints[i].x, controlPoints[i].y,
                   controlPoints[i + 1].x, controlPoints[i + 1].y);
        }
      }

      // Renderizar os pontos de controle
      CV::color(1, 1, 0); // Amarelo
      for (const auto &point : controlPoints) {
        CV::circleFill(point.x, point.y, 5, 10);
      }

      // Indicar os tipos de pontos de controle (P0, P1, P2, P3)
      for (size_t i = 0; i < controlPoints.size(); i++) {
        int pointType = i % 3;
        CV::color(1, 1, 1); // Branco
        char label[10];
        sprintf(label, "P%d", pointType);
        CV::text(controlPoints[i].x + 10, controlPoints[i].y + 10, label);
      }
    }

    // Renderizar a pista (linha central)
    if (trackPoints.size() > 1) {
      CV::color(1, 1, 1); // Branco
      for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        CV::line(trackPoints[i].x, trackPoints[i].y, trackPoints[i + 1].x,
                 trackPoints[i + 1].y);
      }
    }

    // Renderizar os limites da pista
    if (innerBoundary.size() > 1 && outerBoundary.size() > 1) {
      // Limite interno
      CV::color(0.8, 0.2, 0.2); // Vermelho escuro
      for (size_t i = 0; i < innerBoundary.size() - 1; i++) {
        CV::line(innerBoundary[i].x, innerBoundary[i].y, innerBoundary[i + 1].x,
                 innerBoundary[i + 1].y);
      }

      // Limite externo
      CV::color(0.8, 0.2, 0.2); // Vermelho escuro
      for (size_t i = 0; i < outerBoundary.size() - 1; i++) {
        CV::line(outerBoundary[i].x, outerBoundary[i].y, outerBoundary[i + 1].x,
                 outerBoundary[i + 1].y);
      }
    }
  }

  // Define o modo de edição
  void setEditMode(bool mode) { editMode = mode; }

  // Verifica se o modo de edição está ativo
  bool isEditMode() const { return editMode; }

  // Define a largura da pista
  void setTrackWidth(float width) {
    trackWidth = width;
    recalculateTrack();
  }

  // Retorna os pontos da pista para posicionar alvos
  const std::vector<Vector2> &getTrackPoints() const { return trackPoints; }

  // Verifica se a pista tem pontos suficientes para ser válida
  bool isValid() const { return trackPoints.size() >= 4; }

  // Limpa todos os pontos de controle
  void clear() {
    controlPoints.clear();
    trackPoints.clear();
    innerBoundary.clear();
    outerBoundary.clear();
  }
};

#endif // BEZIER_TRACK_H
