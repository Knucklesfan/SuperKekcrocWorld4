#pragma once
#include "scene.h"
#include "util.h"
#include "font.h"
#include "background.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class titlescreen : public scene {
    public:
        titlescreen(SDL_Renderer* render, bg* backg, std::vector <font*> debug, std::vector<Mix_Chunk*> snds, std::vector<Mix_Music*> mus);
        void render(SDL_Renderer* renderer);
        void logic(double deltatime);
        void keyPressed(SDL_Keycode);
        int endlogic();
        void reset() {};
        bg* background;
        std::vector<Mix_Chunk*> chunks;
        std::vector<Mix_Music*> musics;
        std::vector<font*> fonts;

        int currentselection = 0;
        int function = 0;
        std::string titletext = "BIRDPLUMBER ENGINE TEST";
        std::string options[2] = {
            "Play",
            "Exit"
        };
        int functions[2] = {
            0,
            1,
        };

};