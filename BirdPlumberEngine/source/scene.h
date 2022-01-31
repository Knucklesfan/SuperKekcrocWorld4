#pragma once
#include <SDL2/SDL.h>

class scene {
    public:
        virtual void render(SDL_Renderer* renderer) {};
        virtual void logic(double deltatime) {};
        virtual void keyPressed(SDL_Keycode) {};
        virtual int endlogic() = 0;
        virtual void reset() {};
};
