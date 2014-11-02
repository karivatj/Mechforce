#include <iostream>

#include "cEventHandler.h"

bool EventHandler::instanceFlag_ = false;
EventHandler* EventHandler::thisPointer_ = NULL;

EventHandler* EventHandler::getInstance()
{
    if(instanceFlag_)
    {
        return thisPointer_;
    }
    else
    {
        thisPointer_ = new EventHandler();
        instanceFlag_ = true;
        return thisPointer_;
    }
}

EventHandler::EventHandler() : mouse_lastx(0), mouse_lasty(0)
{
}

EventHandler::~EventHandler()
{
    std::cout << "EventHandler: Destroyed" << std::endl;
}

void EventHandler::update()
{
    while(SDL_PollEvent(&event_))
    {
        switch(event_.type)
        {
            case SDL_QUIT:      //X was pressed
                appRunning = false;
            break;

            case SDL_KEYDOWN:
                switch(event_.key.keysym.sym)
                {
                    default:
                    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                MouseButtonPress();
            break;

            case SDL_MOUSEBUTTONUP:
                MouseButtonRelease();
            break;

            case SDL_MOUSEMOTION:
                MouseMotion();
            break;

            case SDL_MOUSEWHEEL:
                MouseWheel();
            break;

            default:
            break;
        }
    }
}

void EventHandler::MouseButtonPress()
{
    if(event_.button.button == SDL_BUTTON_LEFT)  mouseLeftPressed = true;
    if(event_.button.button == SDL_BUTTON_RIGHT) mouseRightPressed = true;
}

void EventHandler::MouseButtonRelease()
{
    if(event_.button.button == SDL_BUTTON_LEFT)  mouseLeftPressed = false;
    if(event_.button.button == SDL_BUTTON_RIGHT) mouseRightPressed = false;
}

void EventHandler::MouseMotion()
{
    ///TODO: FPS Camera
    float diffx = event_.motion.x - mouse_lastx;
    float diffy = event_.motion.y - mouse_lasty;

    mouse_lastx = event_.motion.x;
    mouse_lasty = event_.motion.y;

    if(event_.button.button == SDL_BUTTON_MIDDLE)
    {
        if((rotX += (float) 0.1f * diffy) < 0)
            rotX = 0;
        else if((rotX += (float) 0.1f * diffy) > 90)
            rotX = 90;

        rotY -= (float) 0.1f * diffx;
    }
    if(event_.button.button == SDL_BUTTON_LEFT)
    {
        camX += (float) 0.1f * diffx;
        camY -= (float) 0.1f * diffy;
    }
}

void EventHandler::MouseWheel()
{
    camZ = camZ + (event_.wheel.y);
}

