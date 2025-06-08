#ifndef __BEZIER_CURVE_2D_H__
#define __BEZIER_CURVE_2D_H__

#include "Vector2.h"
#include <math.h>
#include <stdio.h>
#include <vector>

class BCurve2D
{
  private:
    std::vector<Vector2> controlPoints;
    std::vector<Vector2> curvePoints;
    mutable bool needsUpdate; // mutable para permitir atualização em métodos const

    int binomialCoefficient(int n, int k) const;

  public:
    BCurve2D();

    void addControlPoint(Vector2 point);
    void removeControlPoint(int index);
    void updateControlPoint(int index, Vector2 newPos);
    void clearControlPoints();

    void updateCurvePoints();
    Vector2 calculatePoint(float t) const;
    int findNearestControlPoint(const Vector2 &point, float threshold) const;

    void drawCurve() const;
    void drawControlPoints(int selectedIndex = -1) const;

    std::vector<Vector2> getCurvePoints() const { return curvePoints; }
    std::vector<Vector2> getControlPoints() const { return controlPoints; }
    size_t getControlPointCount() const;
    size_t getCurvePointCount() const;
    bool isEmpty() const;
};

#endif
