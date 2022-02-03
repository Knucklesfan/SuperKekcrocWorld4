#pragma once
#include "GameObject.h"
#include "util.h"
#include <cute_c2.h>
#include "block.h"

class Player
{
	public:
		int type = object::PLAYER;
		double x;
		double y;
		SDL_Texture* sprite;
		void render(SDL_Renderer* render, int viewx, int viewy);
		void logic(double deltaTime);
		void physics(double deltatime, std::vector<block*> colliders, int width, std::vector<GameObject*> objects);
		Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render);
		int getx();
		int gety();
		int gettype();
		double xvelocity;
		double yvelocity;
		c2AABB collider;
};

