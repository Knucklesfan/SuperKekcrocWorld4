#pragma once
#include <SDL2/SDL.h>



#include <cute_tiled.h>
#include <vector>
#include "scene.h"
#include "background.h"
#include "util.h"
#include "font.h"
#include "player.h"
#include "block.h"
#include "message.h"
#include "coin.h"
#include "bluecoin.h"
#include "mario.h"

class level : public scene {
    public:
        font* debugfont;
        bg* background;
        std::vector<GameObject*> objects;
        std::vector<Mix_Music*> music;
        std::vector<Mix_Chunk*> sounds;
        block* gettile(int x, int y, int layer);
        SDL_Texture* text;
        level(SDL_Renderer* render, std::string path, bg* backg, font* debug, std::vector<Mix_Chunk*> snds, std::vector<Mix_Music*> mus, std::vector<SDL_Texture*> textures);
        void render(SDL_Renderer* render);
        void logic(double);
        void reset();
        void keyPressed(SDL_Keycode);
        int viewx = 0;
        int viewy = 0;
        int width = 0;
        int height = 0;
        int startx = 0;
        int starty = 0;
        std::vector<std::vector<block*>> actAsVec;
        mplayer* mario;
        Player* player;
        message* messagebox;
        std::vector<std::string> words = {
            "",
        };
        std::vector<std::vector<int>> def = {
            {},
        };

    private:
        cute_tiled_tile_descriptor_t* getTileAt(int, cute_tiled_tile_descriptor_t* tiles);
};