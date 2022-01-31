#include "bgtextscene.h"

bgtextscene::bgtextscene(SDL_Renderer* render, std::vector<SDL_Texture*> texture, std::vector<bg*>  backg, std::vector<Mix_Chunk*> snds, std::vector<font*>  font) {
	
	
	
	
	backgrounds = backg;
	sounds = snds;
	fonts = font;
	msg = new message(render, "default", fonts[0], sounds, words, params, 72, 12, 240, 58);
	currentbg = 0;
	ticks = 0;
	transitionalpha = 1.0;
	down = true;

}
void bgtextscene::render(SDL_Renderer* renderer) {

	backgrounds[currentbg]->render(renderer, false);
	msg->render(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, transitionalpha*255);
	SDL_Rect splashbox = { 0, 0, 400, 240 };
	SDL_RenderFillRect(renderer, &splashbox);

}
void bgtextscene::logic(double deltatime) {

	backgrounds[currentbg]->logic(deltatime);
	msg->logic(deltatime);
	if (ticks >= 10000) {
		msg->active = true;
	}
	if (transitionalpha > 0.0 && down) {
		transitionalpha -= deltatime / 1500;
	}
	else if (transitionalpha <= 0.0 && down) {
		down = false;
		transitionalpha = 0;
	}
	if (transitionalpha < 1.0 && up) {
		transitionalpha += deltatime / 1500;
	}
	else if (transitionalpha >= 1.0 && up) {
		up = false;
	}
	else if (transitioning) {
		currentbg = nextbg;
		down = true;
		transitioning = false;

	}
	ticks += deltatime;
}
void bgtextscene::keyPressed(SDL_Keycode key) {
	if (key == SDLK_e) {
		movebackgrounds(1);
	}
	msg->keyPressed(key);
}
int bgtextscene::endlogic() {
	return 0;
}
void bgtextscene::reset() {

}

//erm, do i call this parameter toMoveto or something? that seems a tad verbose.
//dunno what i wanna do here.
void bgtextscene::movebackgrounds(int tomove) {
	std::cout << "transitioning!\n";
	nextbg = tomove;
	up = true;
	transitioning = true;
}