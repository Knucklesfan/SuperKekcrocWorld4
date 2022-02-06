#include "coin.h"
#include "level.h"
coin::coin(int x, int y, SDL_Texture* text) {
	coin::x = x;
	coin::y = y;
	coin::type = COIN;
	sprite = text;
	active = true;
	box.min = c2V(x+4, y+4-16);
	box.max = c2V(x+4 + 8, y+4 + 8-16);

}

void coin::render(SDL_Renderer* render, int viewx, int viewy) {
	if (active) {

		util::drawTexture(render, sprite, viewx + (x), viewy + (y - 16), 0, 1.0, false, SDL_FLIP_NONE, ((int)frametime % 4) * COINSIZE, 0, 16, 16);
	}
}

void coin::logic(double deltaTime) {
	frametime += deltaTime/100;
}

void coin::collided(GameObject* other, level* lvl) {
	std::cout << "collided!!!";
	active = false;
}

