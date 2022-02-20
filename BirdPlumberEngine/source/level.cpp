#include "level.h"

level::level(SDL_Renderer* render, std::string path, bg* backg, font* debug, std::vector<Mix_Chunk*> snds, std::vector<Mix_Music*> mus, std::vector<SDL_Texture*> textures) {
    cute_tiled_map_t* map = cute_tiled_load_map_from_file((path + "level.json").c_str(), 0);
    cute_tiled_layer_t* layer = map->layers;
    width = map->width;
    height = map->height;
    viewx = 0;
    viewy = 0;
    text = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * map->tilewidth, height * map->tileheight);
    sounds = snds;
    music = mus;
    SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
    //SDL_SetTextureAlphaMod(text, 255);
    SDL_Texture* temp = SDL_GetRenderTarget(render);
    background = backg;
    SDL_SetRenderTarget(render, text);
    SDL_RenderClear(render);
    int wd = 0;
    messagebox = new message(render, "default", debug, sounds, words, def, 72, 12, 240, 58);
    messagebox->loadFromFile("example/first.xml", false);
    while (layer)
    {
        std::vector<block*> vecint;
        SDL_Surface* surf = SDL_LoadBMP(
            (path + "image0.bmp").c_str());
        SDL_Texture* tileset = SDL_CreateTextureFromSurface(render, surf);
        SDL_FreeSurface(surf);
        int* data = layer->data;
        //vecint.push_back(0);
        int data_count = layer->data_count;
        cute_tiled_object_t* object = layer->objects;
        // do something with the tile data
        while (object) {
            if (!strcmp(object->type.ptr, "Player")) {
                player = new Player(object->x, object->y, nullptr, render);
            }
            else if (!strcmp(object->type.ptr, "Coin")) {
                objects.push_back(new coin(object->x, object->y, textures[0]));
            }
            else if (!strcmp(object->type.ptr, "BlueCoin")) {
                objects.push_back(new bluecoin(object->x, object->y, textures[0]));
            }
            object = object->next;
        }

        for (int i = 0; i < data_count; i++) {
            cute_tiled_tile_descriptor_t* tile = getTileAt(data[i] - 1, map->tilesets->tiles);
            if (tile) {
                std::string tadd = tile->type.ptr;
                vecint.push_back(
                    new block(
                        (i % width) * map->tilewidth,
                        (i / width) * map->tileheight,
                        map->tilewidth, map->tileheight,
                        atoi(tadd.c_str()))
                );
            }
            else {
                vecint.push_back(
                    new block(
                        (i % width) * map->tilewidth,
                        (i / width) * map->tileheight,
                        map->tilewidth, map->tileheight,
                        0)
                );
                //std::cout << "{" << vecint.back()->collider.min.x << "," << vecint.back()->collider.min.y << "},{" << vecint.back()->collider.max.x << "," << vecint.back()->collider.max.y << "}\n";
            }
            int posx = viewx + (i % width) * map->tilewidth;
            int posy = viewy + (i / width) * map->tileheight;
            util::drawTexture(render,
                tileset,
                (i % width) * map->tilewidth,
                (i / width) * map->tileheight,
                0,
                1.0,
                false,
                SDL_FLIP_NONE,
                (data[i] % map->tilewidth) * map->tilewidth,
                (data[i] / map->tileheight) * map->tileheight,
                map->tilewidth,
                map->tileheight
            );
            /*
            SDL_Rect splashbox = {(vecint.at(i)->collider.min.x), (vecint.at(i)->collider.min.y), 16, 16};
            SDL_SetRenderDrawColor(render, (vecint.at(i)->actas > 0? 255: 0), 0, 0, 255);
            SDL_RenderDrawRect(render, &splashbox);
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            */
            //debug->render((i % width) * map->tilewidth, (i / width) * map->tileheight, std::to_string(vecint.at(i)->actas), false, render);
        }

        layer = layer->next;
        wd++;
        actAsVec.push_back(vecint);
    }

    SDL_SetRenderTarget(render, temp);
    debugfont = debug;
    level::textures = textures;
}
void level::keyPressed(SDL_Keycode key) {
    if (player != nullptr) {
        player->keyPress(key);
    }
    messagebox->keyPressed(key);

}
void level::render(SDL_Renderer* render) {
    background->render(render, false);
    util::drawTexture(text, viewx, viewy, 0, 1.0, false, SDL_FLIP_NONE, render);
    std::string text = "View: ";
    text += std::to_string(viewx);
    text += " ";
    text += std::to_string(viewy);
    //std::cout << text << "\n";
    for (GameObject* object : objects) {
        object->render(render, viewx, viewy);
    }
    if (player != nullptr) {
        player->render(render, viewx, viewy);
    }
    debugfont->render(8, 8, text, false, render);
    messagebox->render(render);
}

void level::reset() {
    Mix_HaltMusic();
    Mix_PlayMusic(music[1], -1);
}

void level::logic(double deltaTime) {
    if (!messagebox->active) {

        if (player != nullptr) {
            player->preStep(deltaTime);
            player->durangoController(actAsVec.at(1), width, deltaTime);
            player->postStep(deltaTime, objects, this);
            if (-(player->x - 200 < 0)) {
                viewx = 0;
            }
            else {
                viewx = -(player->x - 200);
            }
            viewy = -(player->y - 120);
        }
        for (GameObject* object : objects) {
            object->logic(deltaTime);
            object->phys(deltaTime, actAsVec.at(1), width);
        }

    }
    background->logic(deltaTime);
    messagebox->logic(deltaTime);
    int iter = 0;
    for (GameObject* object : objects) {
        if (!object->active) {
            objects.erase(objects.begin() + iter);
            free(object);
            break;
        }
        iter++;
    }
}
bool level::instance(GameObject* obj) {
     objects.push_back(obj);
     return false;
}
cute_tiled_tile_descriptor_t* level::getTileAt(int index, cute_tiled_tile_descriptor_t* tiles) {

    while (tiles) {
        if (tiles->tile_index == index) {
            return tiles;
        }
        tiles = tiles->next;
    }
    return NULL;
}


block* level::gettile(int x, int y, int layer) {
    int pos = y * width + x % width;
    //std::cout << pos << "\n";
    return actAsVec[layer].at(pos);
}
