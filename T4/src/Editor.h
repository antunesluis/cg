#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "BezierCurve2D.h"
#include "Constants.h"
#include "UIManager.h"
#include "Vector2.h"
#include "Viewer3D.h"
#include <vector>

class Editor
{
  private:
    UIManager uiManager;
    BCurve2D bezierCurve;
    Viewer3D viewer3D;

    bool isDragging;
    int selectedPoint;

    int screenWidth;
    int screenHeight;

    void initializeCurve();
    void drawInfo();
    void handleMouseClick(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void drawDivisionLine();
    void drawAxes2D();

    void updateViewer3D();

    bool isIn2DPanel(int x, int y) const;
    bool isIn3DPanel(int x, int y) const;

    int currentPatchIndex = 0; // Patch atualmente selecionado

  public:
    Editor(int width, int height);
    void initialize();
    void render();
    void update();

    void handleKeyboard(int key);
    void handleKeyboardUp(int key);
    void onMouse(int button, int state, int wheel, int direction, int x, int y);

    void resetCurve();
    void addControlPoint(const Vector2 &point);
    void removeLastControlPoint();
    bool selectControlPoint(const Vector2 &point);
    void moveSelectedPoint(const Vector2 &newPosition);
    void deselectPoint();

    bool isInUIArea(int x, int y) const;
    bool hasSelectedPoint() const { return selectedPoint != -1; }
    bool isEmpty() const;
};

#endif
