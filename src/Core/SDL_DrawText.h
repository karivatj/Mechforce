/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * SDL_DrawText.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef SDL_DRAWTEXT_H
#define SDL_DRAWTEXT_H

#define MAX_TXT_EVENTS  1024
#define MAX_FONTS       2

#include <string>
#include <FTGL/ftgl.h>

FTGL::FTGLfont *font[MAX_FONTS];      /*FTGL Font Array*/

typedef struct {
    int reserved;
    int x;
    int y;
    int r;
    int g;
    int b;
    int size;
    int font;
    char text[128];
} TXT_EVENTS;

TXT_EVENTS text_events[MAX_TXT_EVENTS];

void SDL_DrawText(int size, int x, int y, double r, double g, double b, int font, std::string msg, ...);
void Print(int size, double x, double y, double r, double g, double b, int fontindex, const std::string msg);
int  Font_Cleanup();

#endif /* SDL_DRAWTEXT_H */


