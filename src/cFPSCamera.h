#ifndef FPSCamera_H
#define FPSCamera_H

#include "common.h"

class FPSCamera
{

public:

    FPSCamera(Vector3 pos, Vector3 view, Vector3 up);

    ~FPSCamera();

    void Move(float speed);
    void Rotate(float speed_y, float speed_z);
    void Strafe(float speed);
    void setPosition(Vector3 pos, Vector3 view, Vector3 up);

    Vector3 camPos;
    Vector3 camView;
    Vector3 camUp;
};

#endif // FPSCamera_H
