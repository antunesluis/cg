#include "Layer.h"
#include <cmath>
#include <cstring>

Layer::Layer(int w, int h) : width(w), height(h) {
  image = nullptr;
  originalPixels = nullptr;
  currentPixels = nullptr;
  imageWidth = 0;
  imageHeight = 0;
  visible = true;
  active = false;
  imageX = 0;
  imageY = 0;
  drawingColor[0] = 255;
  drawingColor[1] = 0;
  drawingColor[2] = 0;
}

Layer::~Layer() {
  delete[] originalPixels;
  delete[] currentPixels;
}

void Layer::loadImage(const char *fileName) {
  delete[] originalPixels;
  delete[] currentPixels;
  originalPixels = nullptr;
  currentPixels = nullptr;

  Bmp tempImage(fileName);
  if (!tempImage.getImage())
    return;

  imageWidth = tempImage.getWidth();
  imageHeight = tempImage.getHeight();
  int imgSize = imageWidth * imageHeight * 3;

  originalPixels = new unsigned char[imgSize];
  currentPixels = new unsigned char[imgSize];

  // Convert BGR to RGB while copying
  for (int i = 0; i < imgSize; i += 3) {
    originalPixels[i] = tempImage.getImage()[i + 2];     // R
    originalPixels[i + 1] = tempImage.getImage()[i + 1]; // G
    originalPixels[i + 2] = tempImage.getImage()[i];     // B
  }

  memcpy(currentPixels, originalPixels, imgSize);
}

void Layer::applyEffects(float brightness, float contrast, float saturation) {
  if (!originalPixels)
    return;

  int imgSize = imageWidth * imageHeight * 3;
  memcpy(currentPixels, originalPixels, imgSize);

  applyBrightness(brightness);
  applyContrast(contrast);
  applySaturation(saturation);
}

void Layer::applyBrightness(float brightness) {
  float value = brightness - 1.0f; // Convert to -0.5 to 0.5 range
  int imgSize = imageWidth * imageHeight * 3;

  for (int i = 0; i < imgSize; i++) {
    float pixel = currentPixels[i] / 255.0f;
    pixel = pixel + value;
    currentPixels[i] =
        static_cast<unsigned char>(std::clamp(pixel, 0.0f, 1.0f) * 255);
  }
}

void Layer::applyContrast(float contrast) {
  float factor = contrast * contrast; // More sensitivity at lower values
  int imgSize = imageWidth * imageHeight * 3;

  for (int i = 0; i < imgSize; i++) {
    float pixel = currentPixels[i] / 255.0f;
    pixel = (pixel - 0.5f) * factor + 0.5f;
    currentPixels[i] =
        static_cast<unsigned char>(std::clamp(pixel, 0.0f, 1.0f) * 255);
  }
}

void Layer::applySaturation(float saturation) {
  int imgSize = imageWidth * imageHeight * 3;

  for (int i = 0; i < imgSize; i += 3) {
    float r = currentPixels[i] / 255.0f;
    float g = currentPixels[i + 1] / 255.0f;
    float b = currentPixels[i + 2] / 255.0f;

    float gray = 0.299f * r + 0.587f * g + 0.114f * b;

    r = gray + saturation * (r - gray);
    g = gray + saturation * (g - gray);
    b = gray + saturation * (b - gray);

    currentPixels[i] =
        static_cast<unsigned char>(std::clamp(r, 0.0f, 1.0f) * 255);
    currentPixels[i + 1] =
        static_cast<unsigned char>(std::clamp(g, 0.0f, 1.0f) * 255);
    currentPixels[i + 2] =
        static_cast<unsigned char>(std::clamp(b, 0.0f, 1.0f) * 255);
  }
}

void Layer::applyGaussianBlur(int radius) {
  if (!currentPixels || radius < 1)
    return;

  unsigned char *temp = new unsigned char[imageWidth * imageHeight * 3];
  memcpy(temp, currentPixels, imageWidth * imageHeight * 3);

  // Usa kernel 3x3 para radius=1, 5x5 para radius=2
  const float (*kernel)[5];
  int kernelSize;

  if (radius <= 1) {
    kernel = (const float (*)[5])gaussianKernel3x3;
    kernelSize = 3;
  } else {
    kernel = gaussianKernel5x5;
    kernelSize = 5;
  }

  int margin = kernelSize / 2;

  for (int y = margin; y < imageHeight - margin; y++) {
    for (int x = margin; x < imageWidth - margin; x++) {
      for (int c = 0; c < 3; c++) { // Para cada canal R,G,B
        float sum = 0;

        // Aplica o kernel
        for (int ky = -margin; ky <= margin; ky++) {
          for (int kx = -margin; kx <= margin; kx++) {
            int pos = ((y + ky) * imageWidth + (x + kx)) * 3 + c;
            float weight = kernel[ky + margin][kx + margin];
            sum += temp[pos] * weight;
          }
        }

        int pos = (y * imageWidth + x) * 3 + c;
        currentPixels[pos] = static_cast<unsigned char>(sum);
      }
    }
  }

  delete[] temp;
}

void Layer::render() {
  if (!visible)
    return;
  renderImage(imageX, imageY);
  renderDrawnPixels();
}

void Layer::renderImage(float xOffset, float yOffset) {
  if (!currentPixels)
    return;

  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      int pos = (y * imageWidth + x) * 3;
      float r = currentPixels[pos] / 255.0f;
      float g = currentPixels[pos + 1] / 255.0f;
      float b = currentPixels[pos + 2] / 255.0f;

      CV::color(r, g, b);
      CV::point(x + xOffset, y + yOffset);
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

void Layer::drawPixel(int x, int y, int radius) {
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

void Layer::setDrawingColor(unsigned char r, unsigned char g, unsigned char b) {
  drawingColor[0] = r;
  drawingColor[1] = g;
  drawingColor[2] = b;
}

void Layer::flipHorizontal() {
  if (!currentPixels)
    return;

  int rowSize = imageWidth * 3;
  unsigned char *rowBuffer = new unsigned char[rowSize];

  for (int y = 0; y < imageHeight; y++) {
    unsigned char *rowStart = currentPixels + y * rowSize;
    memcpy(rowBuffer, rowStart, rowSize);

    for (int x = 0; x < imageWidth; x++) {
      int srcPos = x * 3;
      int dstPos = (imageWidth - 1 - x) * 3;
      memcpy(rowStart + dstPos, rowBuffer + srcPos, 3);
    }
  }

  delete[] rowBuffer;
}

void Layer::flipVertical() {
  if (!currentPixels)
    return;

  int rowSize = imageWidth * 3;
  unsigned char *tempBuffer = new unsigned char[imageHeight * rowSize];

  for (int y = 0; y < imageHeight; y++) {
    int srcPos = y * rowSize;
    int dstPos = (imageHeight - 1 - y) * rowSize;
    memcpy(tempBuffer + dstPos, currentPixels + srcPos, rowSize);
  }

  memcpy(currentPixels, tempBuffer, imageHeight * rowSize);
  delete[] tempBuffer;
}

void Layer::convertToGrayscale() {
  if (!currentPixels)
    return;

  int imgSize = imageWidth * imageHeight * 3;
  for (int i = 0; i < imgSize; i += 3) {
    unsigned char gray = static_cast<unsigned char>(
        0.299f * currentPixels[i] + 0.587f * currentPixels[i + 1] +
        0.114f * currentPixels[i + 2]);

    currentPixels[i] = gray;
    currentPixels[i + 1] = gray;
    currentPixels[i + 2] = gray;
  }
}

void Layer::resetEffects() {
  if (originalPixels && currentPixels) {
    int imgSize = imageWidth * imageHeight * 3;
    memcpy(currentPixels, originalPixels, imgSize);
  }
}

void Layer::setPosition(float x, float y) {
  imageX = x;
  imageY = y;
}

void Layer::getPosition(float &outX, float &outY) const {
  outX = imageX;
  outY = imageY;
}

void Layer::clear() {
  pixels.clear();
  pixelColors.clear();
}
