#pragma once
#include <SDL2/SDL.h>



#include <cute_tiled.h>

#include "scene.h"
#include "background.h"
#include "util.h"
#include "font.h"

class level : public scene {
    public:
        font* debugfont;
        SDL_Texture* text;
        level(SDL_Renderer* render, std::string path, bg* backg, font* font);
        void render(SDL_Renderer* render);
        void keyPressed(SDL_Keycode);
        int viewx = 0;
        int viewy = 0;

};