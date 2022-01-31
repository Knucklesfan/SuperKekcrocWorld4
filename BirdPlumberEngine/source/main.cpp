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

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 224

#define TICK_INTERVAL    7

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}



//so like, i totally didnt just absolutely copy nearly everything from Knuxfan's Tetriminos because i'm lazy, noo that'd be stupid lol



int main(int argc, char* argv[])
{
        srand((unsigned)time(0));

        if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
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
        std::vector<bg*> backgs;
        std::vector<SDL_Texture*> textures;
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
        Uint64 NOW = SDL_GetPerformanceCounter();
        Uint64 LAST = 0;
        float deltaTime = 0;
        next_time = SDL_GetTicks() + TICK_INTERVAL;
        double _fps = 0;
        scene* scn = new bgtextscene(renderer, textures, backgs, sounds, fnts );
        while (!quit) {
            auto t1 = std::chrono::high_resolution_clock::now();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    scn->keyPressed(event.key.keysym.sym);
                }
            }

            LAST = NOW;
            NOW = SDL_GetPerformanceCounter();
            deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
            SDL_SetRenderTarget(renderer, rendertext);
            SDL_RenderClear(renderer);


            scn->logic(deltaTime);
            scn->render(renderer);




            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderClear(renderer);


            SDL_RenderCopy(renderer, rendertext, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(time_left());
            next_time += TICK_INTERVAL;

            auto t2 = std::chrono::high_resolution_clock::now();
            Uint64 ms_int = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

            _fps = (1000000 / ms_int);

        }
}