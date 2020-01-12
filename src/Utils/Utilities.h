/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * Utilities.h 1.00 by Kari Vatjus-Anttila
 *
 */
#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H

#include "../main.h"
#include "../Core/SDL_Engine.h"
#include "../Core/SDL_Textures.h"
#include "../Prefs/Preferences.h"
#include "../Widget.h"

/*Variables used in calculating the FPS*/
extern GLfloat fps;
extern GLfloat framestart;

int  Utils_ScreenShot(void); /*Inactive at the moment*/
void Utils_CountFPS(void);
void Utils_WriteConfigFile(void);
void Utils_ReadConfigFile(void);
int  Utils_Toggle_Fullscreen(int width, int height);
STATE Utils_ResolveState(int id);

#endif /*UTILITIES_H*/
