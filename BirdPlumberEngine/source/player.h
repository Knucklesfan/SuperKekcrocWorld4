#pragma once
#include "GameObject.h"
#include "util.h"
#include <cute_c2.h>
#include "block.h"
#include "GameObject.h"
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

		int pose;
		std::vector<int> poses[6] = {
			{0},
			{0,1,2},
			{3},
			{4},
			{5},
			{6}
		};


		bool onground;
		double hsp = 0; //current horizontal speed
		double vsp = 0; //current vertical speed
		int facing = -1;

		const double hsp_max = 64; // max speeds
		const double vsp_max = 128;
		int slope_max = 2;    // Max climb distance
		double accel = 3;
		double fric = 10;
		double grav = 1.0;
private: 
	int objectcollision(int x, int y, int w, int h, GameObject* obj);
	int checkForCollision(int x, int y, int w, int h, std::vector<block*> colliders, int id);
	double approach(double start, double end, double shift);
	int sign(int num);
	int calcSlopeUp(double hsp, std::vector<block*> colliders);
	int calcSlopeDown(double hsp, std::vector<block*> colliders);

};

