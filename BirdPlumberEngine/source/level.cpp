#include "level.h"

level::level(SDL_Renderer* render, std::string path, bg* backg, font* debug) {
    cute_tiled_map_t* map = cute_tiled_load_map_from_file((path+"level.json").c_str(), 0);
    cute_tiled_layer_t* layer = map->layers;
    int w = map->width;
	int h = map->height;
    viewx = 0;
    viewy = 0;
    text = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w*map->tilewidth, h*map->tileheight );
	SDL_Texture* temp = SDL_GetRenderTarget(render);

    SDL_SetRenderTarget(render, text);
    SDL_RenderClear(render);
    int wd = 0;
    while (layer)
	{
        SDL_Texture* tileset = SDL_CreateTextureFromSurface(render,SDL_LoadBMP(
            (path+"image0.bmp").c_str()));
		int* data = layer->data;
		int data_count = layer->data_count;
		// do something with the tile data

        for(int i = 0; i < data_count; i++) {
            int posx = viewx + (i%w)*map->tilewidth;
            int posy = viewy + (i/w)*map->tileheight;
            if(posx <= 400+map->tilewidth*4 && posy <= 224+map->tileheight*4 && posx > 0-map->tilewidth && posy > 0-map->tileheight) {
                util::drawTexture(render,
                tileset,
                viewx + (i%w)*map->tilewidth,
                viewy + (i/w)*map->tileheight,
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
        }

		layer = layer->next;
        wd++;
	}

    SDL_SetRenderTarget(render, temp);
    
}
void level::keyPressed(SDL_Keycode key) {
    switch(key) {
        case SDLK_UP:
            viewy++;
            break;
        case SDLK_DOWN:
            viewx--;
            break;
        case SDLK_LEFT:
            viewy--;
            break;
        case SDLK_RIGHT:
            viewy++;
            break;
    }
}
void level::render(SDL_Renderer* render) {
    util::drawTexture(text,0,0,0,1.0,false,SDL_FLIP_NONE,render);
    std::string text = "View: ";
    text += std::to_string(viewx);
    text += " ";
    text += std::to_string(viewy);
    std::cout << text << "\n";
    debugfont->render(8,8,text,false, render);

}

