#ifndef LAYER_H
#define LAYER_H

#include "Bmp.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <vector>

class Layer {
private:
  // Imagem da camada
  Bmp *image;
  float imageX, imageY;
  // Pixels desenhados manualmente
  std::vector<Vector2> pixels;
  // Cores dos pixels desenhados manualmente (RGB)
  std::vector<unsigned char> pixelColors;
  // Cor atual para desenho
  unsigned char drawingColor[3];

  bool visible;
  bool active;
  int width, height;

public:
  Layer(int w, int h);
  ~Layer();

  void render();
  void renderImage(float xOffset, float yOffset);
  void renderDrawnPixels();
  void loadImage(const char *fileName);

  Bmp *getImage() const { return image; }
  bool hasImage() const;
  void setPosition(float x, float y);
  void getPosition(float &outX, float &outY) const;

  void setVisible(bool v) { visible = v; }
  void setActive(bool a) { active = a; }
  bool isVisible() const { return visible; }
  bool isActive() const { return active; }

  void setDrawingColor(unsigned char r, unsigned char g, unsigned char b);
  void flipHorizontal();
  void flipVertical();
  void adjustBrightness(float factor);
  void clear();

private:
  void drawPixel(int x, int y, int radius = 1);
};

#endif
