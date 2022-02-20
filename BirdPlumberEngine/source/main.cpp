#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "font.h"
#include "background.h"
#include "util.h"
#include "message.h"
#include <chrono>
#include "scene.h"
#include "bgtextscene.h"
#include "GameObject.h"
#include "titlescreen.h"

#define _CRT_SECURE_NO_DEPRECATE
#define CUTE_TILED_IMPLEMENTATION
#define CUTE_C2_IMPLEMENTATION
#include <cute_tiled.h>
#include <cute_c2.h>
#include "level.h"
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 224

#define TICK_INTERVAL    7



double calc_dt()
{
    static int first = 1;
    static double inv_freq;
    static uint64_t prev;

    uint64_t now = SDL_GetPerformanceCounter();

    if (first) {
        first = 0;
        prev = now;
        inv_freq = 1.0 / (double)SDL_GetPerformanceFrequency();
    }

    double dt = ((double)(now - prev) * inv_freq);
    prev = now;
    return dt;
}

//so like, i totally didnt just absolutely copy nearly everything from Knuxfan's Tetriminos because i'm lazy, noo that'd be stupid lol



int main(int argc, char* argv[])
{

        srand((unsigned)time(0));
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"); //LITERALLY NO CLUE WHY THIS WORKS
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

            return 1;
        }

        SDL_Window* window = SDL_CreateWindow("Super Kekcroc World 4", 400, 224, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

            SDL_Quit();

            return 1;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

            SDL_DestroyWindow(window);
            SDL_Quit();

            return 1;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            return 0;
        }
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderSetLogicalSize(renderer, 400, 224);

        SDL_Texture* rendertext = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 400, 224);

        if (SDL_QueryTexture(rendertext, NULL, NULL, NULL, NULL) < 0) {
            printf("TEXTURE ISSUES!!! \n");
            std::cout << SDL_GetError() << "\n";
        };

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();

            return 1;
        }

        std::vector<font*> fnts;
        std::vector<Mix_Chunk*> sounds;
        std::vector<Mix_Music*> music;
        std::vector<bg*> backgs;
        std::vector<SDL_Texture*> textures;
        {
            rapidxml::file<> xmlFile("./sprites/sprites.xml");
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());
            rapidxml::xml_node<char>* parent = doc.first_node("sprites");
            for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
                std::string path = "./sprites/";
                path += child->value();
                SDL_Surface* surf = SDL_LoadBMP(path.c_str());
                SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, surf);
                if (!temp) {
                    printf("Failed to load texture at %s: %s\n", path, SDL_GetError());
                    return 0;
                }
                textures.push_back(temp);
                printf("Successfully loaded texture at %s\n", path.c_str());

            }
        }
        {
            rapidxml::file<> xmlFile("./music/music.xml");
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());
            rapidxml::xml_node<char>* parent = doc.first_node("music");
            for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
                std::string path = "./music/";
                path += child->value();
                Mix_Music* temp = Mix_LoadMUS(path.c_str());
                if (!temp) {
                    printf("Failed to load song at %s: %s\n", path, SDL_GetError());
                    return 0;
                }
                music.push_back(temp);
                printf("Successfully loaded song at %s\n", path.c_str());

            }
        }

        {
            rapidxml::file<> xmlFile("./sounds/sounds.xml");
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());
            rapidxml::xml_node<char>* parent = doc.first_node("sounds");
            for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
                std::string path = "./sounds/";
                path += child->value();
                Mix_Chunk* temp = Mix_LoadWAV(path.c_str());
                if (!temp) {
                    printf("Failed to load sound at %s: %s\n", path, SDL_GetError());
                    return 0;
                }
                sounds.push_back(temp);
                printf("Successfully loaded sound at %s\n", path.c_str());

            }
        }
        {
            rapidxml::file<> xmlFile("./fonts/fonts.xml");
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());
            rapidxml::xml_node<char>* parent = doc.first_node("fonts");
            for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
                font* temp = new font(child->value(), renderer);
                if (!temp) {
                    printf("Failed to load font at %s: %s\n", child->value(), SDL_GetError());
                    return 0;
                }
                fnts.push_back(temp);
                printf("Successfully loaded font at %s\n", child->value());

            }
        }
        {
            rapidxml::file<> xmlFile("./backgrounds/backgrounds.xml");
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());
            rapidxml::xml_node<char>* parent = doc.first_node("backgrounds");
            for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
                bg* temp = new bg(child->value(), false, renderer);
                if (!temp) {
                    printf("Failed to load font at %s: %s\n", child->value(), SDL_GetError());
                    return 0;
                }
                backgs.push_back(temp);
                printf("Successfully loaded font at %s\n", child->value());
            }
        }


        std::vector<std::string> words = {
            "@2Kekcroc:\n@0Hello World! I'm kekcroc, nice to meet you.",
            "@1Mario:\n@0Can you believe it? @4Christmas, just a week away",
            "@2Kekcroc:\n@0This is a very long text. How long? Extremely. Intentionally long, too. If I can't write long text, then what's even the point of being a programmer? Dude, imagine if text was long. There's no way it could be long, Kekcroc. But yeah, but what if it was long. DUUDE! long text is like, a foresaken art, only learned by the greatest of programmers. Yeah dude, this code is crap, there's no way that it can be considered being written by \"the greatest programmer of all time\" or anything lol",
            "@1Mario:\n@2Kekcroc, @0did you @3really @0have to @5break @0the text box @7like that?",
            "@2Kekcroc:\n@0Yes.",

        };
        std::vector<std::vector<int>> def = {
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {2,0,0},
        };

        std::vector<std::string> otherwords = {
            "@2What is your opinion on this game?\n@0   Yes.\n@1   No.\n@0   It's better than nothing.\n@0   Peanut Allergy.\n@0   Fifth Option.",
            "Thanks for your answer.",
            "I HATE YOU.",
            "Least you were honest. Whats 9+10?\n@0   21.\n@0   19\n@3   obama",
            "hehe nut hahah lmao.",
            "Star Wars: Episode V - The Empire Strikes Back",
            "you stupid.",
            "What are you, a math teacher?",
            "'bama"

        };
        std::vector<std::vector<int>> textmods = {
            {1,1,2,3,4,5,5},
            {2,0,0},
            {2,0,0},
            {1,6,7,8,3},
            {2,0,0},
            {2,0,0},
            {2,0,0},
            {2,0,0},
            {2,0,0},

        };
        SDL_Texture* kekcroc = util::generateTexture("./players/kekcroc/0.bmp",renderer);
        SDL_Event event;
        bool quit = false;
        double deltaTime;
        calc_dt();
        int gamemode = 0;
        //scene* scn = new bgtextscene(renderer, textures, backgs, sounds, fnts);
        scene* scenes[3] = {
            new titlescreen(renderer, backgs[1], fnts, sounds, music),
            new bgtextscene(renderer, textures, backgs, sounds, music, fnts),
            new level(renderer,"./levels/testlevels/", backgs.at(0), fnts[0], sounds, music, textures),
        };
        Mix_VolumeMusic(0);
        Mix_HaltMusic();
        Mix_PlayMusic(music[3], -1);
        int lastfps = 0;
        int FPS = 60;
        while (!quit) {
            Uint32 start_time = SDL_GetTicks();
            Uint64 start = SDL_GetPerformanceCounter();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    scenes[gamemode]->keyPressed(event.key.keysym.sym);
                }
            }

            deltaTime = calc_dt();
            SDL_SetRenderTarget(renderer, rendertext);
            SDL_RenderClear(renderer);

            //std::cout << deltaTime;
            scenes[gamemode]->logic(deltaTime);
            scenes[gamemode]->render(renderer);
            int endlogic = scenes[gamemode]->endlogic();
            if (endlogic > 0) {
                gamemode = endlogic;
                scenes[gamemode]->reset();
            }


            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderClear(renderer);

            
            SDL_RenderCopy(renderer, rendertext, NULL, NULL);
            fnts[0]->render(0,0,std::to_string(lastfps),false, renderer);
            SDL_RenderPresent(renderer);
            if (Uint32(1000 / FPS) > (SDL_GetTicks() - start_time))
            {
                SDL_Delay((1000 / FPS) - (SDL_GetTicks() - start_time));
            }
            Uint64 end = SDL_GetPerformanceCounter();
            float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
            lastfps = 1.0f / elapsed;

        }
}