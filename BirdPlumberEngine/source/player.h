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
		int frametime;
		SDL_Texture* sprite;
		void preStep(double deltatime);
		void moveY(std::vector<block*> colliders, int width, double deltaTime);
		void moveX();
		void postStep();
		void render(SDL_Renderer* render, int viewx, int viewy);
		void physics(std::vector<block*> colliders, int width, std::vector<GameObject*> objects);
		Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render);
		int getx();
		int gety();
		int gettype();
		double xvelocity;
		double yvelocity;
		c2AABB collider;
		c2AABB lastbox;
		int pose;
		std::vector<int> poses[6] = {
			{0},
			{0,1,2},
			{3},
			{4},
			{5},
			{6}
		};

};

