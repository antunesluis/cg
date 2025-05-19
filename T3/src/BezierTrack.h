#ifndef BEZIER_TRACK_H
#define BEZIER_TRACK_H

#include "BezierMath.h"
#include "Colors.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <vector>

class BezierTrack {
private:
  std::vector<Vector2> controlPoints;
  std::vector<Vector2> trackPoints; // Linha central da pista
  std::vector<Vector2> innerBoundary;
  std::vector<Vector2> outerBoundary;

  // Largura da pista
  float trackWidth = 200.0f;
  // Número de segmentos para cálculo da curva por seção de Bézier
  const int numSegmentsPerSection = 20;
  // Flag para modo de edição
  bool editMode = false;
  // Flag para indicar se a pista deve ser fechada
  bool closeTrack = true;
  // Propriedades da linha central da estrada
  float centerLineWidth = 6.0f;       // Largura das linhas tracejadas
  float centerLineDashLength = 20.0f; // Comprimento das linhas tracejadas
  float centerLineGapLength = 15.0f;  // Espaço entre as linhas tracejadas

public:
  struct CollisionResult {
    bool collided;
    Vector2 normal;
    Vector2 closestPoint;
    float penetrationDepth;
  };

  BezierTrack() {}

  void recalculateTrack() {
    trackPoints.clear();
    innerBoundary.clear();
    outerBoundary.clear();

    if (controlPoints.size() < 4)
      return;

    int numCurves;
    if (closeTrack && controlPoints.size() >= 4) {
      // Para uma pista fechada, consideramos o último conjunto de pontos
      // conectando de volta ao início
      numCurves = (controlPoints.size()) / 3;
      if (controlPoints.size() % 3 == 1)
        numCurves--; // Ajusta se o número de pontos não for múltiplo de 3
    } else {
      numCurves = (controlPoints.size() - 1) / 3;
    }

    for (int curve = 0; curve < numCurves; curve++) {
      int baseIndex = curve * 3;

      // Ponto inicial (P0)
      const Vector2 &p0 = controlPoints[baseIndex % controlPoints.size()];

      // Pontos de controle (P1, P2)
      const Vector2 &p1 = controlPoints[(baseIndex + 1) % controlPoints.size()];
      const Vector2 &p2 = controlPoints[(baseIndex + 2) % controlPoints.size()];

      // Ponto final (P3) - próximo ponto inicial
      const Vector2 &p3 = controlPoints[(baseIndex + 3) % controlPoints.size()];

      for (int seg = 0; seg <= numSegmentsPerSection; seg++) {
        float t = static_cast<float>(seg) / numSegmentsPerSection;

        Vector2 point = BezierMath::calculatePoint(t, p0, p1, p2, p3);
        Vector2 tangent = BezierMath::calculateTangent(t, p0, p1, p2, p3);

        if (tangent.length() < 0.001f)
          continue;

        Vector2 normal = BezierMath::calculateNormal(tangent);

        trackPoints.push_back(point);
        innerBoundary.push_back(point + normal * (trackWidth / 2));
        outerBoundary.push_back(point - normal * (trackWidth / 2));
      }
    }
  }

  CollisionResult checkCollision(const Vector2 &position, float radius) const {
    CollisionResult result = {false, Vector2(), Vector2(), 0.0f};

    if (innerBoundary.empty() || outerBoundary.empty()) {
      return result;
    }

    // Função para verificar colisão com uma boundary
    auto checkBoundary = [&](const std::vector<Vector2> &boundary) {
      for (size_t i = 0; i < boundary.size() - 1; i++) {
        Vector2 closest =
            closestPointOnSegment(position, boundary[i], boundary[i + 1]);
        float dist = (position - closest).length();

        if (dist < radius) {
          float penetration = radius - dist;
          if (penetration > result.penetrationDepth) {
            result.collided = true;
            result.penetrationDepth = penetration;

            Vector2 normal = position - closest;
            if (normal.length() >
                0.001f) { // Assegura que a normal não é um vetor nulo
              normal.normalize();
              result.normal = normal;
            } else {
              // Se a normal for muito pequena, usa um vetor padrão
              result.normal = Vector2(0, 1);
            }

            result.closestPoint = closest;
          }
        }
      }
    };

    // Verifica colisão com ambos os limites
    checkBoundary(innerBoundary);
    checkBoundary(outerBoundary);

    return result;
  }

  Vector2 closestPointOnSegment(const Vector2 &p, const Vector2 &a,
                                const Vector2 &b) const {
    Vector2 ab = b - a;
    float abLengthSq = ab * ab; // comprimento ao quadrado

    // Evita divisão por zero ou segmentos muito pequenos
    if (abLengthSq < 0.0001f) {
      return a;
    }

    Vector2 ap = p - a;
    float t = (ap * ab) / abLengthSq;
    t = std::max(0.0f, std::min(1.0f, t));
    return a + ab * t;
  }

  bool isInsideTrack(const Vector2 &point) const {
    if (trackPoints.empty() || innerBoundary.empty() || outerBoundary.empty())
      return true;

    // Método simples: verifica se o ponto está fora de todos os limites da
    // pista Distância do ponto ao segmento mais próximo dos limites externos e
    // internos
    float minDistInner = std::numeric_limits<float>::max();
    float minDistOuter = std::numeric_limits<float>::max();

    for (size_t i = 0; i < innerBoundary.size() - 1; i++) {
      float dist =
          distanceToLineSegment(point, innerBoundary[i], innerBoundary[i + 1]);
      minDistInner = std::min(minDistInner, dist);
    }

    for (size_t i = 0; i < outerBoundary.size() - 1; i++) {
      float dist =
          distanceToLineSegment(point, outerBoundary[i], outerBoundary[i + 1]);
      minDistOuter = std::min(minDistOuter, dist);
    }

    // O ponto está dentro da pista se estiver mais próximo do limite externo do
    // que do interno
    return minDistOuter < minDistInner;
  }

  Vector2 getRandomStartPosition() const {
    if (trackPoints.empty())
      return Vector2(100, 100);

    // Encontra um ponto central longe das bordas
    int index = rand() % trackPoints.size();
    return trackPoints[index]; // Retorna um ponto na linha central da pista
  }

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

  // Calcula a distância de um ponto a um segmento de linha
  float distanceToLineSegment(const Vector2 &point, const Vector2 &lineStart,
                              const Vector2 &lineEnd) const {
    Vector2 line = lineEnd - lineStart;
    float lineLengthSq = line * line;

    if (lineLengthSq < 0.0001f) // Segmento muito pequeno, trata como ponto
      return (point - lineStart).length();

    // Projeção do ponto no segmento de linha
    float t = ((point - lineStart) * line) / lineLengthSq;
    t = std::max(0.0f, std::min(1.0f, t));

    Vector2 projection = lineStart + line * t;
    return (point - projection).length();
  }

  // Desenha a linha central tracejada da estrada
  void renderCenterLine() {
    if (trackPoints.size() < 2)
      return;

    Colors::roadCenterMarking();

    // Calcula o comprimento total da pista para determinar as posições das
    // linhas tracejadas
    float totalLength = 0.0f;
    std::vector<float> segmentLengths;

    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
      float length = (trackPoints[i + 1] - trackPoints[i]).length();
      segmentLengths.push_back(length);
      totalLength += length;
    }

    // Desenha linhas tracejadas ao longo do caminho central
    float distanceTraveled = 0.0f;
    float dashStartPos = 0.0f;
    bool drawing = true; // Alterna entre desenhar e não desenhar

    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
      Vector2 start = trackPoints[i];
      Vector2 end = trackPoints[i + 1];
      Vector2 segment = end - start;
      float segmentLength = segmentLengths[i];
      Vector2 direction = segment / segmentLength;

      float segmentPos = 0.0f;

      while (segmentPos < segmentLength) {
        float remainingInSegment = segmentLength - segmentPos;

        if (drawing) {
          // Estamos no período de desenho da linha
          float dashLength =
              std::min(centerLineDashLength - dashStartPos, remainingInSegment);

          Vector2 dashStart = start + direction * segmentPos;
          Vector2 dashEnd = start + direction * (segmentPos + dashLength);

          // Desenha a linha com a largura especificada
          Vector2 perpendicular(-direction.y, direction.x);
          perpendicular.normalize();
          perpendicular = perpendicular * (centerLineWidth / 2.0f);

          float vx[4] = {
              dashStart.x + perpendicular.x, dashStart.x - perpendicular.x,
              dashEnd.x - perpendicular.x, dashEnd.x + perpendicular.x};
          float vy[4] = {
              dashStart.y + perpendicular.y, dashStart.y - perpendicular.y,
              dashEnd.y - perpendicular.y, dashEnd.y + perpendicular.y};

          CV::polygonFill(vx, vy, 4);

          segmentPos += dashLength;
          distanceTraveled += dashLength;

          if (dashStartPos + dashLength >= centerLineDashLength) {
            // Terminamos esta linha tracejada
            drawing = false;
            dashStartPos = 0.0f;
          } else {
            dashStartPos += dashLength;
          }
        } else {
          // Estamos no período de espaço entre linhas
          float gapLength =
              std::min(centerLineGapLength - dashStartPos, remainingInSegment);

          segmentPos += gapLength;
          distanceTraveled += gapLength;

          if (dashStartPos + gapLength >= centerLineGapLength) {
            // Terminamos este espaço
            drawing = true;
            dashStartPos = 0.0f;
          } else {
            dashStartPos += gapLength;
          }
        }
      }
    }
  }

  // Preenche a superfície da pista
  void renderTrackSurface() {
    if (innerBoundary.size() < 2 || outerBoundary.size() < 2)
      return;

    Colors::roadSurface();

    // Percorremos a pista em seções para criar polígonos de preenchimento
    for (size_t i = 0;
         i < innerBoundary.size() - 1 && i < outerBoundary.size() - 1; i++) {
      float vx[4] = {innerBoundary[i].x, innerBoundary[i + 1].x,
                     outerBoundary[i + 1].x, outerBoundary[i].x};
      float vy[4] = {innerBoundary[i].y, innerBoundary[i + 1].y,
                     outerBoundary[i + 1].y, outerBoundary[i].y};

      CV::polygonFill(vx, vy, 4);
    }

    // Se a pista for fechada e tivermos pontos suficientes, fechamos o último
    // segmento
    if (closeTrack && innerBoundary.size() > 2 && outerBoundary.size() > 2) {
      float vx[4] = {innerBoundary[innerBoundary.size() - 1].x,
                     innerBoundary[0].x, outerBoundary[0].x,
                     outerBoundary[outerBoundary.size() - 1].x};
      float vy[4] = {innerBoundary[innerBoundary.size() - 1].y,
                     innerBoundary[0].y, outerBoundary[0].y,
                     outerBoundary[outerBoundary.size() - 1].y};

      CV::polygonFill(vx, vy, 4);
    }
  }

  // Renderiza a pista
  void render() {
    // Primeiro, renderizamos a superfície da pista como um preenchimento
    renderTrackSurface();

    // Desenha os limites da pista
    if (innerBoundary.size() > 1 && outerBoundary.size() > 1) {
      // Limite interno
      Colors::roadEdgeMarking();
      for (size_t i = 0; i < innerBoundary.size() - 1; i++) {
        CV::line(innerBoundary[i].x, innerBoundary[i].y, innerBoundary[i + 1].x,
                 innerBoundary[i + 1].y);
      }

      // Fecha o limite interno se necessário
      if (closeTrack && innerBoundary.size() > 1) {
        CV::line(innerBoundary[innerBoundary.size() - 1].x,
                 innerBoundary[innerBoundary.size() - 1].y, innerBoundary[0].x,
                 innerBoundary[0].y);
      }

      // Limite externo
      Colors::roadEdgeMarking();
      for (size_t i = 0; i < outerBoundary.size() - 1; i++) {
        CV::line(outerBoundary[i].x, outerBoundary[i].y, outerBoundary[i + 1].x,
                 outerBoundary[i + 1].y);
      }

      // Fecha o limite externo se necessário
      if (closeTrack && outerBoundary.size() > 1) {
        CV::line(outerBoundary[outerBoundary.size() - 1].x,
                 outerBoundary[outerBoundary.size() - 1].y, outerBoundary[0].x,
                 outerBoundary[0].y);
      }
    }

    // Desenha as linhas centrais tracejadas da pista
    renderCenterLine();

    // Renderizar pontos de controle (apenas no modo de edição)
    if (editMode) {
      // Conectar os pontos de controle com linhas
      if (controlPoints.size() >= 2) {
        CV::color(0.5, 0.5, 0.5); // Cinza
        for (size_t i = 0; i < controlPoints.size() - 1; i++) {
          CV::line(controlPoints[i].x, controlPoints[i].y,
                   controlPoints[i + 1].x, controlPoints[i + 1].y);
        }

        // Fecha o loop de pontos de controle se necessário
        if (closeTrack && controlPoints.size() >= 3) {
          CV::line(controlPoints[controlPoints.size() - 1].x,
                   controlPoints[controlPoints.size() - 1].y,
                   controlPoints[0].x, controlPoints[0].y);
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

  // Define se a pista deve ser fechada (conectando o último ponto ao primeiro)
  void setCloseTrack(bool close) {
    closeTrack = close;
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
