
#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include "Vector2.h"
#include <math.h>
#include <stdio.h>

class Vector3
{
  public:
    float x, y, z;

    Vector3() { x = y = z = 0; }

    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

#endif
