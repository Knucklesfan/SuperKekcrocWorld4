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

//THE FOLLOWING IS ESPECIALLY THANKS TO ICR ON STACKOVERFLOW
//thank you so much dude for the explaination..
//translated from C# to C++ by me tho
//great read: https://stackoverflow.com/questions/17586/best-word-wrap-algorithm

std::string util::wrap(std::string str, int pixels) {

    std::vector<std::string> words = seperateWords(str, ' ');
    int currentline = 0;
    std::string output = "";
    int currentpix = pixels;
    for (int i = 0; i < words.size(); i++) {
        std::string word = words.at(i);
        int count = 0;
        for (int i = 0; (i = word.find('@', i)) != std::string::npos; i++) {
            count++;
        }
        currentpix += count*2;
        //std::cout << word << count*2 << "\n";
        if (currentline + word.length() > currentpix) {
            if (currentline > 0) {
                currentpix = pixels;
                output += "\n";
                currentline = 1;
            }
            while(word.length() > currentpix) {
                currentpix = pixels;
                output += word.substr(0, currentpix-1) + "";
                word = word.substr(currentpix-1);
                output += "\n";
            }
            word = word.substr(word.find_first_of(' ')+1);
            
        }
        output += word;
        currentline += word.length();
    }

    return output;
}
std::vector<std::string> util::seperateWords(std::string string, char sep) {
    return seperateWords(string, sep, 0);
}
std::vector<std::string> util::seperateWords(std::string string, char sep, int x) {
    std::vector<std::string> parts;
    int startindex = 0;
    while (true) {
        int index = string.find_first_of(sep, startindex);
        if (index == -1) {
            parts.push_back(string.substr(startindex));
            return parts;
        }

        std::string word = string.substr(startindex, index - startindex-x);
        char nextchar = string.substr(index, 1).at(0);
        if (nextchar = sep) {

            parts.push_back(word);
            parts.push_back(std::string(1, nextchar));
        }
        startindex = index + 1;
    }
}
int util::nthOccurrence(const std::string& str, const std::string& findMe, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while (cnt != nth)
    {
        pos += 1;
        pos = str.find(findMe, pos);
        if (pos == std::string::npos)
            return -1;
        cnt++;
    }
    return pos;
}

