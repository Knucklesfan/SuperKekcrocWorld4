#include "level.h"

level::level(SDL_Renderer* render, std::string path, bg* backg, font* debug) {
    cute_tiled_map_t* map = cute_tiled_load_map_from_file((path+"level.json").c_str(), 0);
    cute_tiled_layer_t* layer = map->layers;
    int w = map->width;
	int h = map->height;
    viewx = 0;
    viewy = 0;
    text = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w*map->tilewidth, h*map->tileheight );
    
    SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
    //SDL_SetTextureAlphaMod(text, 255);
    SDL_Texture* temp = SDL_GetRenderTarget(render);
    background = backg;
    SDL_SetRenderTarget(render, text);
    SDL_RenderClear(render);
    int wd = 0;

    while (layer)
	{
        SDL_Surface* surf = SDL_LoadBMP(
            (path + "image0.bmp").c_str());
        SDL_Texture* tileset = SDL_CreateTextureFromSurface(render,surf);
        SDL_FreeSurface(surf);
		int* data = layer->data;
		int data_count = layer->data_count;
        cute_tiled_object_t* object = layer->objects;
		// do something with the tile data
        while (object) {
            if (!strcmp(object->type.ptr, "Player")) {
                player = new Player(object->x, object->y, nullptr, render);
            }
            object = object->next;
        }
        for(int i = 0; i < data_count; i++) {
            int posx = viewx + (i%w)*map->tilewidth;
            int posy = viewy + (i/w)*map->tileheight;
                util::drawTexture(render,
                tileset,
                (i%w)*map->tilewidth,
                (i/w)*map->tileheight,
                0,
                1.0,
                false,
                SDL_FLIP_NONE,
                (data[i]%map->tilewidth)*map->tilewidth,
                (data[i]/map->tileheight)*map->tileheight,
                map->tilewidth,
                map->tileheight
                );
        }

		layer = layer->next;
        wd++;
	}

    SDL_SetRenderTarget(render, temp);
    debugfont = debug;
}
void level::keyPressed(SDL_Keycode key) {

}
void level::render(SDL_Renderer* render) {
    background->render(render, false);
    for (GameObject* object : objects) {
        object->render(render, viewx,viewy);
    }
    util::drawTexture(text, viewx, viewy, 0, 1.0, false, SDL_FLIP_NONE, render);
    std::string text = "View: ";
    text += std::to_string(viewx);
    text += " ";
    text += std::to_string(viewy);
    //std::cout << text << "\n";
    debugfont->render(8,8,text,false, render);

}
void level::logic(double deltaTime) {
    if (player != nullptr) {
        player->logic(deltaTime);
    }
    for (GameObject* object : objects) {
        object->logic(deltaTime);
        std::cout << object->getx() << ", " << object->gety() << "\n";
        //viewx = object->getx()+200;
        //viewy = object->gety()+120;
        if (object->gettype() == object::PLAYER) {
            float lerp = 0.1f;
            viewx = -(object->getx() - 200);
            viewy = -(object->gety() - 120) ;
        }
    }

    

    const Uint8* state = SDL_GetKeyboardState(NULL);
    background->logic(deltaTime);
    if (state[SDL_SCANCODE_W]) {
        viewy++;
    }
    if (state[SDL_SCANCODE_A]) {
        viewx++;
    }
    if (state[SDL_SCANCODE_S]) {
        viewy--;
    }
    if (state[SDL_SCANCODE_D]) {
        viewx--;
    }

}
