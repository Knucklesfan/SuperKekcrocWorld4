#pragma once
#include "GameObject.h"
#include "util.h"
#include <cute_c2.h>
#include "block.h"
#include "GameObject.h"
#include "bullet.h"
class Player
{
	public:
		int type = object::PLAYER;
		double x;
		double y;
		int frametime;
		SDL_Texture* sprite;
		void keyPress(SDL_Keycode key);
		void preStep(double deltatime);
		void postStep(double deltaTime, std::vector<GameObject*> objs, level* level);
		void render(SDL_Renderer* render, int viewx, int viewy);
		void durangoController(std::vector<block*> colliders, int width, double deltaTime);
		Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render);
		const Uint8* prevstate;
		int pose;
		std::vector<int> poses[6] = {
			{0,1,2,3},
			{4,5,6,7,8},
			{4},
			{9},
			{9},
			{9}
		};
		int posespeeds[6] = {
			50,
			25,
			1,
			1,
			1,
			1,
		};


		bool onground;
		double hsp = 0; //current horizontal speed
		double vsp = 0; //current vertical speed
		int facing = -1;

		const double hsp_max = 64; // max speeds
		const double vsp_max = 256;
		int slope_max = 2;    // Max climb distance
		double accel = 10;
		double fric = 50;
		double grav = 10;
		double jumpmod = 0;
private: 
	int objectcollision(int x, int y, int w, int h, GameObject* obj);
	int checkForCollision(int x, int y, int w, int h, std::vector<block*> colliders, int id, int width);
	double approach(double start, double end, double shift);
	int sign(int num);
	int calcSlopeUp(double hsp, std::vector<block*> colliders, int width);
	int calcSlopeDown(double hsp, std::vector<block*> colliders, int width);

};

