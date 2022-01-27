#include "util.h"

void util::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_RendererFlip flip, SDL_Renderer* render) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}

//we do a little trolling, might've stolen some code from background.cpp >:)
SDL_Texture* util::generateTexture(std::string path, SDL_Renderer* renderer) {
        SDL_Surface* surf = SDL_LoadBMP(path.c_str());
        SDL_Texture* ret = SDL_CreateTextureFromSurface(renderer, surf);
        return ret;
}

void util::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_RendererFlip flip, int srcx, int srcy, int srcw, int srch) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, &srcrect, &sprite, angle, NULL, flip);
}

void util::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_RendererFlip flip, int srcx, int srcy, int srcw, int srch, int scalex, int scaley) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = scalex;
    sprite.h = scaley;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x;
        sprite.y = y;
    }
    SDL_RenderCopyEx(renderer, texture, &srcrect, &sprite, angle, NULL, flip);
}

