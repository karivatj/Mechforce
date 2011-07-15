#include <math.h>
#include "Mechforce.h"
#include "SDL_Engine.h"

void Print(int size, double x, double y, double r, double g, double b, int fontindex, const char *msg)
{
    glColor3d(r,g,b);
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
    glLoadIdentity();
    glRasterPos2d(x,y);
    glPushAttrib(GL_LIST_BIT);
    ftglSetFontFaceSize(font[fontindex], size, 72);
    ftglRenderFont(font[fontindex], msg, FTGL_RENDER_ALL);
    glEnable(GL_DEPTH_TEST);
    glPopAttrib();
    glColor3d(1,1,1);
}

void SDL_DrawText(int size, int x, int y, double r, double g, double b, int font, char *msg, ...)
{
    int seek_textevent(void)
    {
        int i;
        for (i=0; i<MAX_TXT_EVENTS; i++)
        {
            if (text_events[i].reserved == 0)
            {
                return(i);
            }
        }
        return(-1);
    }

    char string[128];
    va_list ap;

    va_start(ap, msg);                       // Parses The String For Variables
    vsprintf(string, msg, ap);               // And Converts Symbols To Actual Numbers
    va_end(ap);

    int index = seek_textevent();

    if (index != -1)
    {
        int len;
        len = strlen(string);
        if (len >= 128) len = 127;
        text_events[index].reserved = 1;
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


