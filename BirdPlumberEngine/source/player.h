#pragma once
#include "GameObject.h"
#include "util.h"
#include <cute_c2.h>
#include "block.h"

#define PLAYER_HEIGHT 32.0f
#define PLAYER_HALF_WIDTH 8.0f
#include <cute_math2d.h>

class Player
{
	public:
		int type = object::PLAYER;
		v2 position;
		v2 velocity;
		bool grounded;
		bool jumpable;

		SDL_Texture* sprite;
		void render(SDL_Renderer* render, int viewx, int viewy);
		void logic(double deltaTime);
		void physics(double deltatime, std::vector<block*> colliders, int width, std::vector<GameObject*> objects);
		Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render);
		void player_sync_geometry();
		int getx();
		int gety();
		int gettype();
		aabb_t collider;
		c2Capsule capsule;
		aabb_t c2(c2AABB box) { return make_aabb(c2(box.min), c2(box.max)); }
		c2v c2(v2 v) { return c2V(v.x, v.y);}
		v2 c2(c2v v) { return v2(v.x, v.y); }
		float player_sweep(c2Capsule capsule, v2* n, v2* contact, v2 vel,std::vector<block*> colliders, int width);
		void player_ngs(std::vector<block*> colliders, int width);
		void player_sync_geometry (Player* player);
		int player_can_fall(int pixels_to_fall,std::vector<block*> colliders, int width);
		C2_TYPE tileconv(int id);
};

