#include "message.h"


#ifdef __SWITCH__
#define pth  "/"
#include <switch.h>

#else
#define pth  "./"
#endif

message::message() {};

message::message(SDL_Renderer* renderer, std::string path, font* txt, std::vector<Mix_Chunk*> sound, std::vector<std::string> words, std::vector<std::vector<int>> props, int drax, int dray, int drawid, int drahei) {

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
    setText(words, props);
}
void message::setText(std::vector<std::string> words, std::vector<std::vector<int>> props) {
    towrite.clear();
    textsettings.clear();
    int index = 0;
    for (std::string wrd : words) {
        std::vector<std::string> temp = util::seperateWords(wrd, '\n');
        std::string toadd = "";
        for (std::string ddd : temp) {
            toadd += util::wrap(ddd, drawwid / text->wordsize);
        }
        int count = 0;
        for (int i = 0; (i = toadd.find('\n', i)) != std::string::npos; i++) {
            count++;
        }
        if (count > drawhei / text->height) {
            for (int i = 0; i < count / (drawhei/text->height) + count % (drawhei / text->height); i++) {
                std::string writable = toadd.substr(
                    util::nthOccurrence(toadd, "\n", (i) * (drawhei / text->height)) + (i > 0 ? 1 : 0),
                    util::nthOccurrence(toadd, "\n", (i + 1) * (drawhei / text->height)) - util::nthOccurrence(toadd, "\n", (i) * (drawhei / text->height)));
                if (writable.length() > 2) {
                    towrite.push_back(writable);
                    if (i + 1 >= count / (drawhei / text->height) + count % (drawhei / text->height)) {
                        textsettings.push_back(props.at(index));
                    }
                    else {
                        textsettings.push_back({ 0 });
                    }
                }
            }
            std::cout << count << "\n";
        }
        else {
            textsettings.push_back(props.at(index));
            towrite.push_back(toadd);
        }
        index++;
    }

}


//im just now realizing that 99% of the code that i write for this, i just copy from other places in the same code. Is programming like chipping away at a block of gravel or something?
//wait, im just now realizing i dont know what gravel even is
//what is gravel?
//TODO: find out what gravel is

void message::loadFromFile(std::string file, bool folder) {
    std::vector<std::string> words;
    std::vector<std::vector<int>> props;
    std::string path = "./text/" + file;
    rapidxml::file<> xmlFile(path.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    rapidxml::xml_node<char>* parent = doc.first_node("dialogues");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
        std::string temp = child->value();
        
        words.push_back(temp);
        int type = atoi(child->first_attribute("msgtype")->value());
        std::vector<int> vec;
        vec.push_back(type);
        switch(type) {
            case textsettings::SELECT: {
                int count = atoi(child->first_attribute("size")->value());
                for(int i = 0; i < count; i++) {
                    std::cout << i << "\n";
                    std::string connect = "msg";
                    connect+=std::to_string(i);
                    int var = atoi(child->first_attribute(connect.c_str())->value());
                    vec.push_back(var);
                }
                vec.push_back(count);
            }
        }
        std::cout << "{";
        for(int xd : vec) {
            std::cout << xd << ", ";
        }
        std::cout << "}\n";
        props.push_back(vec);

    }
    std:: cout << words.size() << "\n";
    setText(words,props);
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
            if (textsettings[currentword][0] == textsettings::SELECT) {
                text->render(renderer, "*", drawx + 12, drawy + text->height + 8 + text->height * dialogselect, false, 255, 255, 255);
            }
            else {
                util::drawTexture(renderer, bottomtexture, drawx + drawwid / 2, drawy + drawheight, 0, 1.0, false, SDL_FLIP_NONE, width * 2, 0, width, height);
            }
        }
    }
}
void message::logic(double deltaTime) {
    if (active && !paused) {

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
int message::keyPressed(SDL_Keycode key) {
    if (active) {
        switch (key) {
        case SDLK_x: {
            if (onscrnwrds.length() >= towrite[currentword].length()) {
                return runMsgScript(textsettings[currentword][0]);
            }
            break;
        }
        case SDLK_UP: {
            if (textsettings[currentword][0] == textsettings::SELECT && dialogselect > 0) {
                Mix_PlayChannel(-1, sounds[3], 0);
                dialogselect--;
            }
            break;
        }
        case SDLK_DOWN: {
            if (textsettings[currentword][0] == textsettings::SELECT && dialogselect < textsettings[currentword][textsettings[currentword].size() - 1] - 1) {
                Mix_PlayChannel(-1, sounds[3], 0);
                dialogselect++;
            }
            break;
        }

        }
    }
}

int message::runMsgScript(int type) {
    switch(type) {
        case 2: {
            hide = true;
            currentword = 0;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[2], 0);
            break;
        }
        case 3: {
            hide = true;
            currentword = 0;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[2], 0);
            return 3;
        }
        case 4: {
            currentword++;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[0], 0);
            return 4;
        }

        case textsettings::SELECT: {
            currentword = textsettings[currentword][dialogselect + 1];
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[0], 0);
            break;
        }
        default: {
            currentword++;
            onscrnwrds = "";
            Mix_PlayChannel(-1, sounds[0], 0);
            break;
        }

    }
    return 0;
}