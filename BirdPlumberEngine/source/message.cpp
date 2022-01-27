#include "message.h"


#ifdef __SWITCH__
#define pth  "/"
#include <switch.h>

#else
#define pth  "./"
#endif

message::message() {};

message::message(SDL_Renderer* renderer, std::string path, std::vector<font*> fonts, std::vector<Mix_Chunk*> sound) {
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

}
void message::render(SDL_Renderer* renderer, int drawx, int drawy, int drawwid, int drawhei) {
    util::drawTexture(renderer,filltexture, drawx+width,drawy+height,0,1.0,false,SDL_FLIP_NONE, 0,0,1,1, drawwid-width, drawhei-height);
    util::drawTexture(renderer,toptexture, drawx,drawy,0,1.0,false,SDL_FLIP_NONE, 0,0,width,height);
    util::drawTexture(renderer,bottomtexture, drawx,drawy+drawhei,0,1.0,false,SDL_FLIP_NONE, 0,0,width,height);
    int i = width;
    for(; i < drawwid; i+=8) {
        util::drawTexture(renderer,toptexture, drawx+i,drawy,0,1.0,false, SDL_FLIP_NONE, width,0,width,height);
        util::drawTexture(renderer,bottomtexture, drawx+i,drawy+drawhei,0,1.0,false, SDL_FLIP_NONE, width,0,width,height);
    }
    util::drawTexture(renderer,toptexture, drawx+i,drawy,0,1.0,false,SDL_FLIP_HORIZONTAL, 0,0,width,height);
    util::drawTexture(renderer,bottomtexture, drawx+i,drawy+drawhei,0,1.0,false,SDL_FLIP_HORIZONTAL, 0,0,width,height);
    
    
    
    
    util::drawTexture(renderer,sidetexture, drawx,drawy+height,0,1.0,false,SDL_FLIP_NONE, 0,0,width,1, width, drawhei-height);
    util::drawTexture(renderer,sidetexture, drawx+i,drawy+height,0,1.0,false,SDL_FLIP_HORIZONTAL, 0,0,width,1, width, drawhei-height);

}
