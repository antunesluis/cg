#ifndef POLYGON_H
#define POLYGON_H

#include "Vector2.h"
#include "gl_canvas2d.h"
#include <vector>

class Polygon {
private:
  std::vector<Vector2> originalVertices;
  std::vector<Vector2> transformedVertices;

public:
  Polygon() = default;

  // Cria um retângulo centralizado
  void createRectangle(float width, float height) {
    // Cria um retângulo centralizado na origem
    originalVertices.clear();
    float halfW = width / 2;
    float halfH = height / 2;

    originalVertices = {Vector2(-halfW, -halfH), Vector2(-halfW, halfH),
                        Vector2(halfW, halfH), Vector2(halfW, -halfH)};

    transformedVertices = originalVertices;
  }

  // Aplica transformações (rotação e translação)
  void transform(float angle, const Vector2 &position) {
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotaciona cada vértice em torno da origem usando matriz de rotação
    for (size_t i = 0; i < originalVertices.size(); i++) {
      // Rotaciona
      float x = originalVertices[i].x * cosA - originalVertices[i].y * sinA;
      float y = originalVertices[i].x * sinA + originalVertices[i].y * cosA;

      // Translada
      transformedVertices[i].x = x + position.x;
      transformedVertices[i].y = y + position.y;
    }
  }

  // Desenha o polígono preenchido
  void fill() const {
    if (transformedVertices.size() < 3)
      return;

    std::vector<float> vx, vy;
    for (const auto &v : transformedVertices) {
      vx.push_back(v.x);
      vy.push_back(v.y);
    }

    CV::polygonFill(vx.data(), vy.data(), (int)vx.size());
  }
};

#endif
