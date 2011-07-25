#include <stdio.h>
#include "Mechforce.h"
#include "Button.h"
#include "SDL_Engine.h"

int BTN_ReadButtonData(void)
{
    FILE *file;
    char *token = NULL;
    char string[128];
    int counter1 = 0;
    int counter2 = 0;
    int done = 0;
    int length;
    int temp;

    if((file = fopen ("../Data/buttons.txt", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open buttons datafile (./Data/Buttons.txt)\n");
        SDL_Close(-1);
    }

    printf("Succesfully opened button datafile.\nPreparing to read button data.\n");

    while(!feof(file))
    {
        counter2 = 0;
        fgets(string, 128, file);
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

            switch(counter2)
            {
                case 0: Buttons[counter1].type = temp; break;
                case 1: Buttons[counter1].size = temp; break;
                case 2: Buttons[counter1].group = temp; break;
                case 3: Buttons[counter1].x = temp; break;
                case 4: Buttons[counter1].y = temp; break;
                case 5: Buttons[counter1].state = ResolveState(temp); break;
                case 6: Buttons[counter1].targetstate = ResolveState(temp); break;
                case 7: strncpy(Buttons[counter1].caption,token,length); break;

                default:
                break;
            }

            token = strtok (NULL, ",");
            counter2++;
        }

        counter1++;

        if(counter1 > MAX_BUTTONS || done)
        {
            printf("Finished reading buttons.\n\n");
            break;
        }

        printf("Button read succesfully. Type = %d and caption = %s. [%d/%d]\n", Buttons[counter1-1].type, Buttons[counter1-1].caption, counter1-1, MAX_BUTTONS);
    }

    fclose (file);

    return 0;   //OK
}

void BTN_HandleButtonStateChanges(SDL_EventType eventtype, int button, int buttonstate, int x, int y)
{
    int i;
    float xstride, ystride;

    for(i = 0; i < MAX_BUTTONS; i++)
    {
        if(Buttons[i].state == state)   /*Check if the button is located in the state the user is in*/
        {
            if(Buttons[i].type == BIG_RECT_BTN)
            {
                xstride = 25.8 * Buttons[i].size;
                ystride = 4.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == SMALL_RECT_BTN)
            {
                xstride = 12.8 * Buttons[i].size;
                ystride = 4.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == RADIOBUTTON)
            {
                xstride = 8.2 * Buttons[i].size;
                ystride = 8.2 * Buttons[i].size;
            }
            else if(Buttons[i].type == CHECKBOX)
            {
                xstride = 10 * Buttons[i].size;
                ystride = 10 * Buttons[i].size;
            }
            if(x >= Buttons[i].x + 2 && x <= Buttons[i].x + xstride && y >= Buttons[i].y + 2 && y <= Buttons[i].y + ystride) //If the cursor is inside the button
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
            else if(Buttons[i].type  == 0 || Buttons[i].type == 1)
            {
                Buttons[i].enabled = 0;
                Buttons[i].mouseover = 0;
            }
        }
    }
}

BUTTON BTN_HandlePresses(BUTTON b, int button)
{
    int i;

    switch(b.type)
    {
        case BIG_RECT_BTN:
        case SMALL_RECT_BTN:
            if(b.enabled == 0)
            {
                SDL_PlaySound(1);
                b.enabled = 1;
            }
        break;

        case RADIOBUTTON:
                b.enabled = 1 - b.enabled;
                SDL_PlaySound(3);

                for(i = 0; i < MAX_BUTTONS; i++)
                {
                    if(Buttons[i].state == state && Buttons[i].group == b.group)
                        Buttons[i].enabled = 0;
                }
        break;

        case CHECKBOX:
                b.enabled = 1 - b.enabled;
                SDL_PlaySound(3);
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
        case BIG_RECT_BTN:
        case SMALL_RECT_BTN:
            state = b.targetstate;
            SDL_PlaySound(3);
            SDL_Delay(250);
            break;

        case RADIOBUTTON:
        case CHECKBOX:
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
        case BIG_RECT_BTN:
        case SMALL_RECT_BTN:
        if(b.enabled == 0)
        {
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

        case RADIOBUTTON:
        break;

        case CHECKBOX:
        break;

        default:
        break;
    }
    return b;
}

void BTN_DrawButtonScene(void)
{
    int i, stride;

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

        if(Buttons[i].state == state)
        {
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            glLoadIdentity();
            glTranslated(Buttons[i].x, Buttons[i].y, 0);

            if(Buttons[i].type == 2 || Buttons[i].type == 3)
            glBindTexture(GL_TEXTURE_2D, buttontextures[Buttons[i].type * 3 + stride + 2]);

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

    ftglSetFontFaceSize(font[0], 25 - (10 - b.size), 72);
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
