#include "titlescreen.h"

titlescreen::titlescreen(SDL_Renderer* render, bg* backg, std::vector <font*> debug, std::vector<Mix_Chunk*> snds, std::vector<Mix_Music*> mus) {
	background = backg;
	musics = mus;
	chunks = snds;
	fonts = debug;
}
void titlescreen::render(SDL_Renderer* render) {
	background->render(render, false);
	SDL_Rect splashbox = { 256, 0, 144, 224 };
	SDL_SetRenderDrawColor(render, 0, 0, 0, 128);
	SDL_RenderFillRect(render, &splashbox);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderDrawRect(render, &splashbox);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 128);
	fonts[0]->render(128-32, 104, "@2Kekcroc", false, render);
	fonts[0]->render(8, 120, "And The Legendary Quest For One Coin", false, render);
	int i = 0;
	fonts[0]->render(368, 32 + (i * 16), "GAME MENU", true, render);
	for (std::string str : options) {
		if (currentselection == i) {
			str = "@2" + str;
		}
		fonts[0]->render(render, str, 320, 64 + (i * 16), false);
		i++;
	}
}

void titlescreen::logic(double deltatime) {
	background->logic(deltatime);
}
int titlescreen::endlogic() {
	return function;
}
void titlescreen::keyPressed(SDL_Keycode keys) {
	switch (keys) {
		case SDLK_UP: {
			if (currentselection > 0) {
				currentselection--;
			}
			break;
		}
		case SDLK_DOWN: {
			if (currentselection < 1) {
				currentselection++;
			}
			break;
		}
		case SDLK_z: {
			switch (functions[currentselection]) {
				case 0: {
					function = 1;
					break;
				}
				case 1: {
					exit(0);
				}
			}
		}
	}
}
