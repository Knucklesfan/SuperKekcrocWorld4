#include "bluecoin.h"
#include "level.h"

bluecoin::bluecoin(int x, int y, SDL_Texture* text) {
	bluecoin::x = x;
	bluecoin::y = y;
	bluecoin::type = COIN;
	sprite = text;
	active = true;
	box.min = c2V(x + 4, y + 4);
	box.max = c2V(x + 4 + 8, y + 4 + 8);

}

void bluecoin::collided(GameObject* other, level* lvl) {
	std::cout << "collided!!!";

	Mix_HaltMusic();
	Mix_PlayMusic(lvl->music[2],-1);
	lvl->messagebox->active = true;
	active = false;
}
void bluecoin::render(SDL_Renderer* render, int viewx, int viewy) {
	if (active) {
		SDL_SetTextureColorMod(sprite, 0, 0, 255);
		util::drawTexture(render, sprite, viewx + (x), viewy + (y - 16), 0, 1.0, false, SDL_FLIP_NONE, ((int)frametime % 4) * COINSIZE, 0, 16, 16);
		SDL_SetTextureColorMod(sprite, 255, 255, 255);
	}
}
