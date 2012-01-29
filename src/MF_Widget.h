#ifndef BUTTON_H
#define BUTTON_H

#define MAX_BUTTONS 15

/*Button type enum*/
typedef enum {
    BIG_RECT_BTN,
    SMALL_RECT_BTN,
    RADIOBUTTON,
    CHECKBOX,
    TEXT
} Widget;

/*Button datatype*/
typedef struct {
    Widget type;
    int size;
    int group;
    int enabled;
    int mouseover;
    int x;
    int y;
    char caption[32];
    STATE state;
    STATE targetstate;
} BUTTON;

BUTTON Buttons[MAX_BUTTONS];

int BTN_ReadButtonData(void);
void BTN_HandleButtonStateChanges(SDL_EventType eventtype, int button, int buttonstate, int x, int y);
void BTN_DrawButtonScene(void);
void BTN_DrawButton(BUTTON b);
BUTTON BTN_HandlePresses(BUTTON b, int button);
BUTTON BTN_HandleReleases(BUTTON b);
BUTTON BTN_HandleMouseOvers(BUTTON b, int button);

#endif /*BUTTON_H*/