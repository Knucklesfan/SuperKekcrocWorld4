#pragma once
#include "font.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include "util.h"
class message {
    public:
        message();
        message(SDL_Renderer* renderer, std::string path, std::vector<font*> textures, std::vector<Mix_Chunk*> sound);
        void render(SDL_Renderer* renderer, int drawx, int drawy, int drawwid, int drawhei);
        int width = 0;
        int height = 0;
        int pressa = 0;
        bool doanimation = false;
        bool doblip = false;
    private:
        font* header;
        font* text;
        SDL_Texture* toptexture;
        SDL_Texture* bottomtexture;
        SDL_Texture* sidetexture;
        SDL_Texture* filltexture;
        SDL_Texture* rendertexture;

};