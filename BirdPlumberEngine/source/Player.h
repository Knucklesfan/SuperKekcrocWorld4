#pragma once
#include "GameObject.h"
#include "util.h"
class Player : public GameObject
{
	public:
		int type = object::PLAYER;
		int x;
		int y;
		SDL_Texture* sprite;
		void render(SDL_Renderer* render, int viewx, int viewy);
		void logic(double deltaTime);
		Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render);
		int getx();
		int gety();
		int gettype();
};
