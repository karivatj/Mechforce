#ifndef SDL_UTILS_H
#define SDL_UTLIS_H

int  Utils_ScreenShot(void); /*Inactive at the moment*/
void Utils_CountFPS(void);
void Utils_WriteConfigFile(void);
void Utils_ReadConfigFile(void);
STATE Utils_ResolveState(int id);

#endif /*SDL_UTILS_H*/
