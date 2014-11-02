#ifndef EventHandler_H
#define EventHandler_H

#include "cGame.h"

class EventHandler
{
public:

    static EventHandler* getInstance();

    void update();

    void setOwner(Game *g) { owner_ = g; }

protected:

    EventHandler();

    ~EventHandler();

private:

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
