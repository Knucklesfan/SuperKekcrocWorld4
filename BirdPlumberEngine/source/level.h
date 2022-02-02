#pragma once
#include <SDL2/SDL.h>



#include <cute_tiled.h>
#include <vector>
#include "scene.h"
#include "GameObject.h"
#include "background.h"
#include "util.h"
#include "font.h"
#include "player.h"

class level : public scene {
    public:
        font* debugfont;
        bg* background;
        std::vector<GameObject*> objects;
        SDL_Texture* text;
        level(SDL_Renderer* render, std::string path, bg* backg, font* font);
        void render(SDL_Renderer* render);
        void logic(double);
        void keyPressed(SDL_Keycode);
        int viewx = 0;
        int viewy = 0;
        Player* player;
};