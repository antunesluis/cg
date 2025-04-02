#include "Layer.h"
#include "gl_canvas2d.h"

Layer::Layer(int w, int h) {
  image = nullptr;
  width = w;
  height = h;
  visible = true;
  active = false;

  drawingColor[0] = 255; // R
  drawingColor[1] = 0;   // G
  drawingColor[2] = 0;   // B
}

Layer::~Layer() {
  if (image != nullptr) {
    delete image;
  }
}

void Layer::loadImage(const char *fileName) {
  // Se já existir uma imagem, deleta ela
  if (image != nullptr) {
    delete image;
  }

  image = new Bmp(fileName);
  if (image != nullptr) {
    image->convertBGRtoRGB();
    printf("\nImagem carregada: %s (%dx%d)", fileName, image->getWidth(),
           image->getHeight());
  }
}

void Layer::drawPixel(int x, int y, int radius) {
  // Desenha um quadrado ao redor do ponto central x, y de acordo com o raio
  for (int dy = -radius; dy <= radius; dy++) {
    for (int dx = -radius; dx <= radius; dx++) {
      int px = x + dx;
      int py = y + dy;

      if (px >= 0 && px < width && py >= 0 && py < height) {
        pixels.emplace_back(px, py);
        pixelColors.insert(pixelColors.end(),
                           {drawingColor[0], drawingColor[1], drawingColor[2]});
      }
    }
  }
}

void Layer::render() {
  if (!visible)
    return;

  renderImage();
  renderDrawnPixels();
}

void Layer::renderImage() {
  if (image == nullptr || image->getImage() == nullptr) {
    printf("\nErro: Imagem inválida na camada");
    return;
  }

  unsigned char *imgData = image->getImage();
  const int imgWidth = image->getWidth();
  const int imgHeight = image->getHeight();
  const int bytesPerLine = image->bytesPerLine;

  for (int y = 0; y < imgHeight; y++) {
    for (int x = 0; x < imgWidth; x++) {
      int pos = y * bytesPerLine + x * 3;

      float r = imgData[pos] / 255.0f;
      float g = imgData[pos + 1] / 255.0f;
      float b = imgData[pos + 2] / 255.0f;

      CV::color(r, g, b);
      CV::point(x, y);
    }
  }
}

void Layer::renderDrawnPixels() {
  for (size_t i = 0; i < pixels.size(); i++) {
    CV::color(pixelColors[i * 3] / 255.0f, pixelColors[i * 3 + 1] / 255.0f,
              pixelColors[i * 3 + 2] / 255.0f);
    CV::point(pixels[i].x, pixels[i].y);
  }
}

void Layer::setDrawingColor(unsigned char r, unsigned char g, unsigned char b) {
  drawingColor[0] = r;
  drawingColor[1] = g;
  drawingColor[2] = b;
}

void Layer::flipHorizontal() {
  if (image != nullptr) {
    // Implementar flip horizontal da imagem
  }
}

void Layer::flipVertical() {
  if (image != nullptr) {
    // Implementar flip vertical da imagem
  }
}

void Layer::adjustBrightness(float factor) {
  if (image != nullptr) {
    // Implementar ajuste de brilho
  }
}

void Layer::clear() {
  pixels.clear();
  pixelColors.clear();
}
