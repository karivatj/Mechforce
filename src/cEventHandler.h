#ifndef EventHandler_H
#define EventHandler_H

#include <SDL2/SDL.h>

class Game;

class EventHandler
{
public:

    static EventHandler* getInstance();

    ~EventHandler();

    void update();

    void setOwner(Game *g);

private:

    EventHandler();

    static EventHandler     *thisPointer_;

    static bool             instanceFlag_;

    SDL_Event               event_;

    Game                    *owner_;

    int                     mouse_lastx;
    int                     mouse_lasty;
    bool                    mouseLeftPressed;
    bool                    mouseRightPressed;

    void MouseButtonPress();
    void MouseButtonRelease();
    void MouseMotion();
    void MouseWheel();
};

#endif //EventHandler_H
