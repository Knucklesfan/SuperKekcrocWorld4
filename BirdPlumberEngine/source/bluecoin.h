#pragma once
#include "coin.h"

class bluecoin : public coin {
	public:
		bluecoin(int x, int y, SDL_Texture* text);
		void collided(GameObject* object, level* lvl);
		void render(SDL_Renderer* render, int viewx, int viewy);
};