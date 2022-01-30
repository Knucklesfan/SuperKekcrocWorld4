#include "message.h"


#ifdef __SWITCH__
#define pth  "/"
#include <switch.h>

#else
#define pth  "./"
#endif

message::message() {};

message::message(SDL_Renderer* renderer, std::string path, font* txt, std::vector<Mix_Chunk*> sound, std::vector<std::string> words, int drax, int dray, int drawid, int drahei) {

    std::string filepath = pth "sprites/messageboxes/" + path + "/";
    rapidxml::file<> xmlFile((filepath+"theme.xml").c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    std::cout << "Preparing messagebox: " << path << "..."; 
    width = atoi(doc.first_node("width")->value());
    height = atoi(doc.first_node("height")->value());
    toptexture = util::generateTexture(filepath+doc.first_node("topsprite")->value(), renderer);
    bottomtexture = util::generateTexture(filepath+doc.first_node("bottomsprite")->value(), renderer);
    sidetexture = util::generateTexture(filepath+doc.first_node("sidesprite")->value(), renderer);
    filltexture = util::generateTexture(filepath+doc.first_node("fillsprite")->value(), renderer);
    onscrnwrds = "";
    sounds = sound;
    text = txt;
    drawx = drax;
    drawy = dray;
    drawwid = drawid;
    drawhei = drahei;
    drawheight = height;
    setText(words);
}

void message::setText(std::vector<std::string> words) {
    towrite.clear();
    for (std::string wrd : words) {
        std::vector<std::string> temp = util::seperateWords(wrd, '\n');
        std::string toadd = "";
        for (std::string ddd : temp) {
            
            toadd += util::wrap(ddd, drawwid / text->wordsize);
        }
        towrite.push_back(toadd);
    }

}

void message::render(SDL_Renderer* renderer) {
    if (active) {
        util::drawTexture(renderer, filltexture, drawx + width, drawy + height, 0, 1.0, false, SDL_FLIP_NONE, 0, 0, 1, 1, drawwid - width, drawheight - height);
        util::drawTexture(renderer, toptexture, drawx, drawy, 0, 1.0, false, SDL_FLIP_NONE, 0, 0, width, height);
        util::drawTexture(renderer, bottomtexture, drawx, drawy + drawheight, 0, 1.0, false, SDL_FLIP_NONE, 0, 0, width, height);
        int i = width;
        for (; i < drawwid; i += 8) {
            util::drawTexture(renderer, toptexture, drawx + i, drawy, 0, 1.0, false, SDL_FLIP_NONE, width, 0, width, height);
            util::drawTexture(renderer, bottomtexture, drawx + i, drawy + drawheight, 0, 1.0, false, SDL_FLIP_NONE, width, 0, width, height);
        }
        util::drawTexture(renderer, toptexture, drawx + i, drawy, 0, 1.0, false, SDL_FLIP_HORIZONTAL, 0, 0, width, height);
        util::drawTexture(renderer, bottomtexture, drawx + i, drawy + drawheight, 0, 1.0, false, SDL_FLIP_HORIZONTAL, 0, 0, width, height);




        util::drawTexture(renderer, sidetexture, drawx, drawy + height, 0, 1.0, false, SDL_FLIP_NONE, 0, 0, width, 1, width, drawheight - height);
        util::drawTexture(renderer, sidetexture, drawx + i, drawy + height, 0, 1.0, false, SDL_FLIP_HORIZONTAL, 0, 0, width, 1, width, drawheight - height);

        text->render(renderer, onscrnwrds, drawx + 12, drawy + 8, false, 255, 255, 255);

        if (onscrnwrds.length() >= towrite[currentword].length()) {
            util::drawTexture(renderer, bottomtexture, drawx + drawwid / 2, drawy + drawheight, 0, 1.0, false, SDL_FLIP_NONE, width * 2, 0, width, height);
        }
    }
}
void message::logic(double deltaTime) {
    if (active) {

        if (!expand || hide) {
            if (hide) {
                drawheight -= deltaTime / 2.5;
                if (drawheight <= height) {
                    expand = false;
                    hide = false;
                    active = false;

                }
            }
            else {
                drawheight += deltaTime / 2.5;
                if (drawheight >= drawhei) {
                    expand = true;
                }
            }
        }
        else {
            if (onscrnwrds.length() < towrite[currentword].length()) {
                bliptime += deltaTime;
                if (bliptime >= 10) {
                    if (towrite[currentword].at(onscrnwrds.length()) != '\n' || towrite[currentword].at(onscrnwrds.length()) != ' ') {
                        Mix_PlayChannel(1, sounds[1], 0);
                    }
                    onscrnwrds += towrite[currentword].at(onscrnwrds.length());
                    bliptime = 0;
                }
            }
        }
    }
}
void message::keyPressed(SDL_Keycode key) {
    if (onscrnwrds.length() >= towrite[currentword].length()) {
        if (currentword < towrite.size()-1) {
            currentword++;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[0], 0);
        }
        else {
            hide = true;
            currentword = 0;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[2], 0);
        }

    }
}