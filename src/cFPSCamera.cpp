#include "cFPSCamera.h"
#include "math.h"

FPSCamera::FPSCamera(Vector3 pos, Vector3 view, Vector3 up) :
    camPos(pos),
    camView(view),
    camUp(up)
{

}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::Move(float speed)
{
    Vector3 vVector = camView - camPos;	// Get the view Vector3
    camPos.x  = camPos.x  + vVector.x * speed;
    camPos.z  = camPos.z  + vVector.z * speed;
    camView.x = camView.x + vVector.x * speed;
    camView.z = camView.z + vVector.z * speed;
}

void FPSCamera::Rotate(float speed_y, float speed_z)
{
    // The higher the value is the faster the FPSCamera looks around.
    camView.y += speed_z * 2;

    // limit the rotation around the x-axis
    if((camView.y - camPos.y) > 8)  camView.y = camPos.y + 8;
    if((camView.y - camPos.y) <-8)  camView.y = camPos.y - 8;

    Vector3 vVector = camView - camPos;	// Get the view Vector3

    camView.z = (float)(camPos.z + sin(-speed_y)*vVector.x + cos(-speed_y)*vVector.z);
    camView.x = (float)(camPos.x + cos(-speed_y)*vVector.x - sin(-speed_y)*vVector.z);

    //printf("View x %f, View y %f, View z %f\n", camView.x, camView.y, camView.z);
}

void FPSCamera::Strafe(float speed)
{
    Vector3 vVector = camView - camPos;
    Vector3 vOrthoVector;                // Orthogonal Vector3 for the view Vector3

    vOrthoVector.x = -vVector.z;
    vOrthoVector.z =  vVector.x;

    camPos.x  = camPos.x  + vOrthoVector.x * speed;
    camPos.z  = camPos.z  + vOrthoVector.z * speed;
    camView.x = camView.x + vOrthoVector.x * speed;
    camView.z = camView.z + vOrthoVector.z * speed;
}

void FPSCamera::setPosition(Vector3 pos, Vector3 view, Vector3 up)
{
    camPos = pos;
    camView = view;
    camUp = up;
}
