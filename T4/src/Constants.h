#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Vector2.h"
#include <vector>

namespace Constants
{
// Aplicação
const float SCREEN_WIDTH = 1480.0;
const float SCREEN_HEIGHT = 820.0;
const float PANEL_WIDTH = SCREEN_WIDTH / 2;
const float ROTATION_SPEED = 2.0;

// Bézier
const int DEFAULT_NUM_SAMPLES = 100.0;
const int CONTROL_POINT_RADIUS = 6.0f;
const int SELECTION_THRESHOLD = 10.0f;

// Objeto 3D
const int MIN_ROTATION_STEPS = 3;
const int MAX_ROTATION_STEPS = 50;
const int DEFAULT_ROTATION_STEPS = 20;

// Projeção 3D
const float DEFAULT_FOV = 60.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;
const float PROJECTION_SCALE = 200.0f;

// Câmera
const float CAMERA_DISTANCE = 300.0f;
const float ROTATION_SENSITIVITY = 0.01f;

} // namespace Constants

#endif
