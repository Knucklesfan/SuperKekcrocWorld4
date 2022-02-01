#pragma once
#include <SDL2/SDL.h>
#include <vector>
enum object{
	PLAYER = 0,
	ENEMY = 1,
};
class GameObject {
	public:
		int type;
		int x;
		int y;
		SDL_Texture* sprite;
		virtual void render(SDL_Renderer* render, int viewx, int viewy) {};
		virtual void logic(double deltaTime) {};
		virtual void phys(double deltaTime, std::vector<int> colliders) {};
		virtual void collided(GameObject other) {};
		virtual int getx() { return 0; };
		virtual int gety() { return 0; };
		virtual int gettype() { return 0; }
};