/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * SDL_DrawText.c 1.00 by Kari Vatjus-Anttila
 *
 */

#include <math.h>
#include "../main.h"
#include "SDL_Engine.h"
#include "SDL_DrawText.h"

FTGL::FTGLfont *font[MAX_FONTS];        /*FTGL Font Array*/
TXT_EVENTS text_events[MAX_TXT_EVENTS];

void Print(int size, double x, double y, double r, double g, double b, int fontindex, std::string msg)
{
    glColor3d(r,g,b);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
    glLoadIdentity();
    glRasterPos2d(x,y);
    glPushAttrib(GL_LIST_BIT);
    ftglSetFontFaceSize(font[fontindex], size, 72);
    ftglRenderFont(font[fontindex], msg.c_str(), FTGL::RENDER_ALL);
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
    glColor3d(1,1,1);
}

int seek_textevent(void)
{
    int i;
    for (i = 0; i < MAX_TXT_EVENTS; i++)
    {
        if (text_events[i].reserved == FALSE)
            return(i);
    }
    return(-1);
}

void SDL_DrawText(int size, int x, int y, double r, double g, double b, int font, std::string msg, ...)
{
    char string[128];
    va_list ap;

    va_start(ap, msg);                       // Parses The String For Variables
    vsprintf(string, msg.c_str(), ap);               // And Converts Symbols To Actual Numbers
    va_end(ap);

    int index = seek_textevent();

    if (index != -1)
    {
        int len;
        len = strlen(string);
        if (len >= 128) len = 127;
        text_events[index].reserved = TRUE;
        text_events[index].x = x;
        text_events[index].y = y;
        text_events[index].r = r;
        text_events[index].g = g;
        text_events[index].b = b;
        text_events[index].size = size;
        text_events[index].font = font;
        memcpy(text_events[index].text, string, len);
        text_events[index].text[len] = '\0';
    }
}

int Font_Cleanup()
{
    printf("Freeing fonts...\n");
    if(font[0] != NULL && font[1] != NULL)
    {
        ftglDestroyFont(font[0]);
        ftglDestroyFont(font[1]);
        return 0;
    }
    else
    return -1;
}
