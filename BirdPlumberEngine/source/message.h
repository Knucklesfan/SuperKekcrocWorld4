#pragma once
#include "font.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include "util.h"
#include <string>
enum textsettings {
    SELECT=1, 
    END = 2,
};

class message {
    public:

        message();
        message(SDL_Renderer* renderer, std::string path, font* txt, std::vector<Mix_Chunk*> sound, std::vector<std::string>, std::vector<std::vector<int>> props, int drax, int dray, int drawid, int drahei);
        void render(SDL_Renderer* renderer);
        void logic(double deltaTime);
        void keyPressed(SDL_Keycode key);
        void setText(std::vector<std::string> words, std::vector<std::vector<int>>);
        bool active = false;
        bool expand = false;
        bool hide = false;
        int width = 0;
        int height = 0;
        int pressa = 0;
        int dialogselect = 0;
        int currentword = 0;
        int drawheight = 0;
        bool doanimation = false;
        bool doblip = false;
        std::vector<std::string> towrite;
        std::string onscrnwrds;
        std::vector<std::vector<int>> textsettings;
        double bliptime = 0;
        int drawx, drawy, drawwid, drawhei;
    private:
        font* text;
        SDL_Texture* toptexture;
        SDL_Texture* bottomtexture;
        SDL_Texture* sidetexture;
        SDL_Texture* filltexture;
        SDL_Texture* rendertexture;
        std::vector<Mix_Chunk*> sounds;
};