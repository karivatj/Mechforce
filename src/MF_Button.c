#include <stdio.h>
#include "Mechforce.h"
#include "Button.h"
#include "SDL_Engine.h"

int BTN_ReadButtonData(void)
{
    STATE ResolveState(int id)
    {
        switch(id)
        {
            case 0:  return STATE_QUIT;     break;
            case 1:  return STATE_MAINMENU; break;
            case 2:  return STATE_GAME;     break;
            case 3:  return STATE_OPTIONS;  break;
            case -1: return STATE_NULL;     break;
            default: return STATE_NULL;     break;
        }
    }

    FILE *tiedosto;
    char *token = NULL;
    char string[128];
    int laskuri1 = 0;
    int laskuri2 = 0;
    int done = 0;
    int length;
    int temp;

    if((tiedosto = fopen ("../Data/buttons.txt", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open buttons datafile (./Data/Buttons.txt)\n");
        SDL_Close(-1);
    }

    printf("Succesfully opened button datafile.\nPreparing to read button data.\n");

    while(!feof(tiedosto))
    {
        laskuri2 = 0;
        fgets(string, 128, tiedosto);
        token = strtok (string,",");

        while (token != NULL)
        {
            if(strcmp(token,"END") == 0)
            {
                done = 1;
                break;
            }

            temp = atoi(token);
            length = strlen(token);

            switch(laskuri2)
            {
                case 0: Buttons[laskuri1].type = temp; break;
                case 1: Buttons[laskuri1].size = temp; break;
                case 2: Buttons[laskuri1].group = temp; break;
                case 3: Buttons[laskuri1].x = temp; break;
                case 4: Buttons[laskuri1].y = temp; break;
                case 5: Buttons[laskuri1].tila = ResolveState(temp); break;
                case 6: Buttons[laskuri1].kohdetila = ResolveState(temp); break;
                case 7: strncpy(Buttons[laskuri1].caption,token,length); break;

                default:
                break;
            }

            token = strtok (NULL, ",");
            laskuri2++;
        }

        laskuri1++;

        if(laskuri1 > MAX_BUTTONS || done)
        {
            printf("Finished reading buttons.\n\n");
            break;
        }

        printf("Button read succesfully. Type = %d and caption = %s. [%d/%d]\n", Buttons[laskuri1-1].type, Buttons[laskuri1-1].caption, laskuri1-1, MAX_BUTTONS);
    }

    fclose (tiedosto);

    return 0;   //OK
}

void BTN_HandleButtonStateChanges(SDL_EventType eventtype, int button, int buttonstate, int x, int y)
{
    int i;
    float xstride, ystride;

    for(i = 0; i < MAX_BUTTONS; i++)
    {
        if(Buttons[i].tila == state)
        {
            if(Buttons[i].type == 0) {
                xstride = 25.8 * Buttons[i].size;
                ystride = 4.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == 1) {
                xstride = 12.8 * Buttons[i].size;
                ystride = 4.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == 2) {
                xstride = 8.2 * Buttons[i].size;
                ystride = 8.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == 3) {
                xstride = 10 * Buttons[i].size;
                ystride = 10 * Buttons[i].size;
            }
            if(x >= Buttons[i].x + 2 && x <= Buttons[i].x + xstride && y >= Buttons[i].y + 2 && y <= Buttons[i].y + ystride) //Jos kursori on napin sisällä
            {
                switch(eventtype)
                {
                    case SDL_MOUSEMOTION:
                    Buttons[i] = BTN_HandleMouseOvers(Buttons[i], button);
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                    Buttons[i] = BTN_HandlePresses(Buttons[i], button);
                    break;

                    case SDL_MOUSEBUTTONUP:
                    Buttons[i] = BTN_HandleReleases(Buttons[i]);
                    break;

                    default:
                    break;
                }
            }
            else
            {
                if(Buttons[i].type  == 0 || Buttons[i].type == 1) {
                Buttons[i].enabled = 0;
                Buttons[i].mouseover = 0;
                }
            }
        }
    }
}

BUTTON BTN_HandlePresses(BUTTON b, int button)
{
    int i;

    switch(b.type)
    {
        case 0:
        case 1:
            if(button == SDL_BUTTON_LEFT && b.enabled == 0)
            {
                SDL_PlaySound(1);
                b.enabled = 1;
            }
        break;

        case 2:
            if(button == SDL_BUTTON_LEFT)
            {
                b.enabled = 1 - b.enabled;
                SDL_PlaySound(3);

                for(i = 0; i < MAX_BUTTONS; i++)
                {
                    if(Buttons[i].tila == state && Buttons[i].group == b.group)
                    {
                        Buttons[i].enabled = 0;
                    }
                }
            }
        break;

        case 3:
            if(button == SDL_BUTTON_LEFT)
            {
                b.enabled = 1 - b.enabled;
                SDL_PlaySound(3);
            }
        break;

        default:
        break;
    }
    return b;
}
BUTTON BTN_HandleReleases(BUTTON b)
{
    switch(b.type)
    {
        case 0:
        case 1:
            state = b.kohdetila;
            SDL_PlaySound(3);
            SDL_Delay(500);
            break;

        case 2:
        case 3:
        break;

        default:
        break;
    }
    return b;
}
BUTTON BTN_HandleMouseOvers(BUTTON b, int button)
{
    switch(b.type)
    {
        case 0:
        case 1:
        if(button == 1 && b.enabled == 0)
        {  //Jos hiiren vasenta nappia painetaan ja nappi ei ole vielä aktivoitu
            b.enabled = 1;
            b.mouseover = 1;
            SDL_PlaySound(1);
        }
        else if(b.mouseover == 0)
        {
            b.mouseover = 1;
            SDL_PlaySound(2);
        }
        break;

        case 2:
        break;

        case 3:
        break;

        default:
        break;
    }
    return b;
}

void BTN_DrawButtonScene(void)
{
    int i,stride;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    for(i = 0; i < MAX_BUTTONS; i++)
    {
        if(Buttons[i].type == 0)
        stride = 0;

        else if(Buttons[i].type == 1)
        stride = 1;

        else
        stride = 2;

        if(Buttons[i].tila == state)
        {
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            glLoadIdentity();
            glTranslated(Buttons[i].x, Buttons[i].y,0);

            if(Buttons[i].type == 2 || Buttons[i].type == 3)
            glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride + 2]);

            else
            glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride + 3]);

            BTN_DrawButton(Buttons[i]);

            glBlendFunc( GL_ONE, GL_ONE );

            if(Buttons[i].enabled == 1)
            {
                if(Buttons[i].type == 2 || Buttons[i].type == 3)
                glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride + 1]);

                else
                glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride + 2]);

                BTN_DrawButton(Buttons[i]);
            }

            else if(Buttons[i].mouseover == 1 && (Buttons[i].type == 0 || Buttons[i].type == 1))
            {
                glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride + 1]);
                BTN_DrawButton(Buttons[i]);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D,buttontextures[Buttons[i].type * 3 + stride]);
                BTN_DrawButton(Buttons[i]);
            }
        }
    }

    glDisable(GL_BLEND);
    glEnable( GL_DEPTH_TEST );
}

void BTN_DrawButton(BUTTON b)
{
    float height, length;

    ftglSetFontFaceSize(font[0],25 - (10 - b.size), 72);
    length = ftglGetFontAdvance(font[0], b.caption);
    height = ftglGetFontAscender(font[0]) + ftglGetFontDescender(font[0]);

    switch(b.type)
    {
        case 0:
        glBegin(GL_QUADS);
            glTexCoord2d(0,1);        glVertex2d(0 , 0);
            glTexCoord2d(1,1);        glVertex2d(26 * b.size, 0);
            glTexCoord2d(1,0);        glVertex2d(26 * b.size, 4.4 * b.size);
            glTexCoord2d(0,0);        glVertex2d(0 , 4.4 * b.size);
        glEnd();
        SDL_DrawText(25 - (10-b.size),b.x + ((25.8 * b.size) / 2 - (length / 2)) ,b.y + ((4.4 * b.size) / 2 - (height / 2)),1,1,1,0,b.caption);
        break;

        case 1:
        glBegin(GL_QUADS);
            glTexCoord2d(0,1);        glVertex2d(0 , 0);
            glTexCoord2d(1,1);        glVertex2d(13 * b.size , 0);
            glTexCoord2d(1,0);        glVertex2d(13 * b.size, 4.4 * b.size);
            glTexCoord2d(0,0);        glVertex2d(0 , 4.4 * b.size);
        glEnd();
        SDL_DrawText(25 - (10-b.size),b.x + ((13 * b.size) / 2 - (length / 2)) ,b.y + ((4.4 * b.size) / 2 - (height / 2)),1,1,1,0,b.caption);
        break;

        case 2:
        glBegin(GL_QUADS);
            glTexCoord2d(0,1);        glVertex2d(0 , 0);
            glTexCoord2d(1,1);        glVertex2d(8.2 * b.size, 0);
            glTexCoord2d(1,0);        glVertex2d(8.2 * b.size, 8.2 * b.size);
            glTexCoord2d(0,0);        glVertex2d(0 , 8.2 * b.size);
        glEnd();
        SDL_DrawText(25 - (10-b.size),b.x + 2 + (8.2 * b.size) ,b.y + ((8.2 * b.size) / 2 - (height / 2)),1,1,1,0,b.caption);
        break;

        case 3:
        glBegin(GL_QUADS);
            glTexCoord2d(0,1);        glVertex2d(0 , 0);
            glTexCoord2d(1,1);        glVertex2d(10 * b.size, 0);
            glTexCoord2d(1,0);        glVertex2d(10 * b.size, 10 * b.size);
            glTexCoord2d(0,0);        glVertex2d(0 , 10 * b.size);
        glEnd();
        SDL_DrawText(25 - (10-b.size),b.x + 2 + (10 * b.size) ,b.y + ((10 * b.size) / 2 - (height / 2)),1,1,1,0,b.caption);
        break;

        default:
        break;
    }
}
