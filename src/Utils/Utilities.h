#ifndef UTILITIES_H
#define UTILITIES_H

#include "../Mechforce.h"

int  Utils_ScreenShot(void); /*Inactive at the moment*/
void Utils_CountFPS(void);
void Utils_WriteConfigFile(void);
void Utils_ReadConfigFile(void);
STATE Utils_ResolveState(int id);

#endif /*UTILITIES_H*/
