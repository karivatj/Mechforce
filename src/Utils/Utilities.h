/*
 * Mechforce.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "GL/gl.h"
#include "../main.h"

/*Variables used in calculating the FPS*/
GLfloat fps;
GLfloat framestart;

int  Utils_ScreenShot(void); /*Inactive at the moment*/
void Utils_CountFPS(void);
void Utils_WriteConfigFile(void);
void Utils_ReadConfigFile(void);
STATE Utils_ResolveState(int id);

#endif /*UTILITIES_H*/
