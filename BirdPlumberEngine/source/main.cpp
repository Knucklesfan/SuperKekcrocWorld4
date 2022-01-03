#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "font.h"
#include "background.h"
#include "util.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 224


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


        font* fnt = new font("small8x8font", renderer);
        bg* backg = new bg("cavestory", false, renderer);
        SDL_Surface* surf = SDL_LoadBMP("./players/kekcroc/0.bmp");
        SDL_Texture* kekcroc = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Event event;
        bool quit = false;
        Uint64 NOW = SDL_GetPerformanceCounter();
        Uint64 LAST = 0;
        double deltaTime = 0;

        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {

                }
            }

            LAST = NOW;
            NOW = SDL_GetPerformanceCounter();
            deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
            backg->logic(deltaTime);
            SDL_SetRenderTarget(renderer, rendertext);
            SDL_RenderClear(renderer);

            backg->render(renderer, false);
            fnt->render(200, 112, "KEKCROC WORLD 4", true, renderer);
            fnt->render(200, 128, "Developed by Knuxfan", true, renderer);
            util::drawTexture(kekcroc, 200, 180, 0, 1.0, false, renderer);
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderClear(renderer);


            SDL_RenderCopy(renderer, rendertext, NULL, NULL);
            SDL_RenderPresent(renderer);

        }
}
