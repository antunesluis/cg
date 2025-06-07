#ifndef LAYER_H
#define LAYER_H

#include "Bmp.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <vector>

template <typename T> const T &clamp(const T &value, const T &low, const T &high)
{
    return (value < low) ? low : (high < value) ? high : value;
}

class Layer
{
  private:
    static constexpr float gaussianKernel3x3[3][3] = {
        {1 / 16.0f, 2 / 16.0f, 1 / 16.0f}, {2 / 16.0f, 4 / 16.0f, 2 / 16.0f}, {1 / 16.0f, 2 / 16.0f, 1 / 16.0f}};

    static constexpr float gaussianKernel5x5[5][5] = {{1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f},
                                                      {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
                                                      {6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f},
                                                      {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
                                                      {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f}};

    Bmp *image;
    unsigned char *originalPixels;
    unsigned char *currentPixels;
    int imageWidth, imageHeight;

    // Pixels desenhados manualmente
    std::vector<Vector2> pixels;
    std::vector<unsigned char> pixelColors;

    unsigned char drawingColor[3];
    bool visible;
    bool active;
    int width, height;
    float imageX, imageY;

    void renderDrawnPixels();
    void applyBrightness(float brightness);
    void applyContrast(float contrast);
    void applySaturation(float saturation);

  public:
    Layer(int w, int h);
    ~Layer();

    void loadImage(const char *fileName);
    void applyEffects(float brightness, float contrast, float saturation);
    void applyGaussianBlur(int radius);

    void render();
    void renderImage(float xOffset, float yOffset);
    void drawPixel(int x, int y, int radius = 1);

    int getImageWidth() const { return imageWidth; }
    int getImageHeight() const { return imageHeight; }

    void setPosition(float x, float y);
    void getPosition(float &outX, float &outY) const;
    bool hasImage() const { return originalPixels != nullptr; }

    void setVisible(bool v) { visible = v; }
    void setActive(bool a) { active = a; }
    bool isVisible() const { return visible; }
    bool isActive() const { return active; }

    void setDrawingColor(unsigned char r, unsigned char g, unsigned char b);
    void flipHorizontal();
    void flipVertical();
    void convertToGrayscale();
    void clear();
    void resetEffects();
};

#endif
