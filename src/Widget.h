/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * Widget.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef WIDGET_H
#define WIDGET_H

#define MAX_BUTTONS 15

/*Button type enum*/
typedef enum {
    BIG_RECT_BTN,
    SMALL_RECT_BTN,
    RADIOBUTTON,
    CHECKBOX,
    TEXT
} WidgetType;

/*Button datatype*/
typedef struct {
    WidgetType type;
    int size;
    int group;
    int enabled;
    int mouseover;
    int x;
    int y;
    char caption[32];
    STATE state;
    STATE targetstate;
} Widget;

Widget Buttons[MAX_BUTTONS];

int Widget_ReadButtonData(void);
void Widget_HandleButtonStateChanges(SDL_EventType eventtype, int button, int buttonstate, int x, int y);
void Widget_DrawButtonScene(void);
void Widget_DrawButton(Widget b);
Widget Widget_HandlePresses(Widget b, int button);
Widget Widget_HandleReleases(Widget b);
Widget Widget_HandleMouseOvers(Widget b, int button);

#endif /*WIDGET_H*/
