#include "font.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <map>
#include <utility>      // std::pair, std::make_pair
#include <math.h>



#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif


font::font() {

}

//this is um, also copied from background.h and modified lmao
//I sWeAr I'm A gOoD cOdEr gUyS

font::font(std::string path, SDL_Renderer* renderer) {
    std::string p = "fonts/" + path;
    generateSurfaces(p, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!

    std::string pth = filepath "fonts/" + path + "/fontdef.xml";

    rapidxml::file<> xmlFile(pth.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    width = atoi(doc.first_node("width")->value());
    height = atoi(doc.first_node("height")->value());
    size = atoi(doc.first_node("row")->value()); 
    if (doc.first_node("spwidth") != NULL) {
        wordsize = atoi(doc.first_node("spwidth")->value());
    }
    else {
        wordsize = width;
    }
    rapidxml::xml_node<char>* parent = doc.first_node("letters");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
        letter let;

        let.x = atoi(child->first_node("posx")->value());
        let.y = atoi(child->first_node("posy")->value());
        let.width = atoi(child->first_node("charwid")->value());
        let.character = child->first_node("char")->value()[0];
        mapping[let.character] = let;
    }



    //DEBUG ONLY STUFF, YKNOW HOW IT IS
    // thanks to geeks4geeks for being a great code source, i might or might not have copied this to save time but whatever it's literally just a debug function so who cares
    
    
    std::map<char, letter>::iterator itr;
    std::cout << "\nThe map is : \n";
    std::cout << "\tKEY\tELEMENT\n";
    for (itr = mapping.begin(); itr != mapping.end(); ++itr) {
        std::cout << '\t' << itr->first
             << '\t' << itr->second.character << '\t' << itr->second.width << '\t' << itr->second.x << '\t' << itr->second.y << '\n';
    }
    std::cout << "\n";
    
    doc.clear();
}
void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer) {
    render(renderer, strg, x, y, center, 0, 0, 0);
}
void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, bool sine, double pos, double multiplyin, double multiplyout) {
    render(renderer, strg, x, y, center, 0, 0, 0, 0, sine, pos, multiplyin, multiplyout);
}

void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, int wordwrap) {
    render(renderer, strg, x, y, center, 0, 0, 0, wordwrap);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green) {
    render(renderer, words, x, y, center, red, blue, green, 0);
}
void font::render(SDL_Renderer* renderer, std::string strg, int x, int y, bool center) {
    render(renderer, strg, x, y, center, 0, 0, 0);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap) {
    render(renderer, words, x, y, center, red, blue, green, wordwrap, false, 0, 0, 0);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout) {
    int finalwidth = 0;
    int drawcolor = 0;
    if(center) {
        if (wordwrap > 0 && words.length()*width > wordwrap) {
            finalwidth = wordwrap;
        }
        else {
            finalwidth = words.length() * width;
        }
    }
    if(red > 0 || blue > 0 || green > 0) {
        SDL_SetTextureColorMod( texture, red, blue, green );

    }
    if(wordwrap > 0) {
        words = util::wrap(words, wordwrap/ wordsize);
    }

    double tmpy = y;
    int tmpx = x-finalwidth/2;
    int i = 0;
    for(char& c : words) {
        if (i >= 1 && words.at(i-1) == '@') {
            i++;
            continue;
        }
        SDL_SetTextureColorMod(texture, colors[drawcolor].r, colors[drawcolor].g, colors[drawcolor].b);
        char& a = c;
        if (mapping.find(c) == mapping.end()) {
            a = std::toupper(c);
        }
        if (mapping.find(a) != mapping.end()) {
            if (sine) {
                tmpy = y + (sin((pos + i) * multiplyin) * multiplyout);
            }
            drawTexture(renderer, texture, tmpx, tmpy, 0, 1.0, false, mapping.at(a).x * width, mapping.at(a).y * height, mapping.at(a).width, height);
            tmpx += mapping.at(a).width;
        }
        else {
            //std::cout << "LOADED BAD CHAR!!\n";
            if (a == '\n') {
                tmpy += height;
                tmpx = x - finalwidth / 2;
            }
            else if (a == '@') {
                if (i + 1 < words.length()) {
                    drawcolor = atoi(&words.at(i + 1));
                    i++;
                }
                continue;
            }
            else {
                tmpx += wordsize;
            }
        }
        i++;
    }
    SDL_SetTextureColorMod(texture, 255,255,255);

}
//uses the modern drawtexture from background.h, hopefully there's no bugs?

//had to add a few more parameters becauuuuuse rendering sprite sheets is our new intention with drawTexture

void font::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
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
    SDL_RenderCopy(renderer, texture, &srcrect, &sprite);
}

bool font::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}


//ALSO STOLEN FROM BACKGROUND.H LMAOOOO

//i still dont understand how this works, and that's terrifying because i entirely wrote this myself

//update: stealing from background.h is morally incorrect so i decided instead to recode this since fonts can literally only have one texture

//ADD MORE TEXTURES TO A FONT AND I'LL BREAK YOU LEGS


//now that i mention it, adding more than one texture to a font could be cool? but i'm not really designing an rpg text box, am i?

//...or am i?


void  font::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    std::string tmppth = path + "/font.bmp";
    SDL_Surface* tmpsurf = SDL_LoadBMP(tmppth.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
    SDL_FreeSurface(tmpsurf);
}

// i seriously considered this a rewrite lmao ^^^



