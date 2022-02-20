#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cute_c2.h>
#include "block.h"
enum object {
	PLAYER = 0,
	ENEMY = 1,
	COIN = 2,
};
class level;

class GameObject {
	public:
		c2AABB box;
		int type;
		int x;
		int y;
		bool active;
		SDL_Texture* sprite;
		virtual void render(SDL_Renderer* render, int viewx, int viewy) {};
		virtual void logic(double deltaTime) {};
		virtual void phys(double deltaTime, std::vector<block*> colliders, int width) {};
		virtual void collided(GameObject* object, level* level) {};
		virtual int getx() { return 0; };
		virtual int gety() { return 0; };
		virtual int gettype() { return 0; }
		virtual bool getactive() { return false; }
		virtual c2AABB getbox() { return box; }

};