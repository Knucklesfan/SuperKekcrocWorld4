#include "player.h"

Player::Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render) {
    type = object::PLAYER;
	position.x = xa;
	position.y = ya;
	sprite = sheet;
    SDL_Surface* surf = SDL_LoadBMP("./players/kekcroc/0.bmp");
    sprite = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
}
void Player::render(SDL_Renderer* render, int viewx, int viewy) {
	util::drawTexture(sprite, viewx+position.x, viewy+position.y, 0, 1.0, false, SDL_FLIP_NONE, render);
}
void Player::logic(double deltaTime) {
    //std::cout << "(" << x << "," << y << ")\n";
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        velocity.y = -1;
    }
    else if (state[SDL_SCANCODE_DOWN]) {
        velocity.y = 1;
    }
    else {
        velocity.y -= deltaTime/500;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        velocity.x = 1;
    }
    else if (state[SDL_SCANCODE_LEFT]) {
        velocity.x = -1;
    }
    else {
        velocity.x = 0;
    }
}

void Player::physics(double deltatime, std::vector<block*> colliders, int width, std::vector<GameObject*> objects) {
    
}
int Player::getx() {
    return position.x;
}

int Player::gety() {
    return position.y;
}

int Player::gettype() {
    return type;
}

void Player::player_sync_geometry ()
{
	capsule.a = c2(position + v2(0, PLAYER_HEIGHT / 2.0f - capsule.r));
	capsule.a.y += 2.0f;
	capsule.b = c2(position + v2(0, -PLAYER_HEIGHT / 2.0f + capsule.r));
	collider = make_aabb(position, PLAYER_HALF_WIDTH * 2.0f, PLAYER_HEIGHT);
}

void Player::player_sync_geometry (Player* player)
{
	player->capsule.a = c2(position + v2(0, PLAYER_HEIGHT / 2.0f - capsule.r));
	player->capsule.a.y += 2.0f;
	player->capsule.b = c2(position + v2(0, -PLAYER_HEIGHT / 2.0f + capsule.r));
	player->collider = make_aabb(position, PLAYER_HALF_WIDTH * 2.0f, PLAYER_HEIGHT);
}

float Player::player_sweep(c2Capsule capsule, v2* n, v2* contact, v2 vel,std::vector<block*> colliders, int width) {
    float min_toi = 1;
    v2 min_toi_normal;
    v2 min_toi_contact;

    for(int i = 0; i < colliders.size(); i++) {
        int x = i % width;
        int y = i / width;
        int id = colliders.at(i)->actas;
        if(id > 0) {
            v2 toi_normal;
			v2 toi_contact;
			int iters;
            float toi = c2TOI(&colliders.at(i)->collider, tileconv(id), 0, c2V(0, 0), &capsule, C2_TYPE_CAPSULE, 0, c2(vel), 1, (c2v*)&toi_normal, (c2v*)&toi_contact, &iters);
            if(toi < min_toi) {
                min_toi = toi;
				min_toi_normal = toi_normal;
				min_toi_contact = toi_contact;
            }
        }
    }
    *n = min_toi_normal;
	*contact = min_toi_contact;
	return min_toi;
}

void Player::player_ngs(std::vector<block*> colliders, int width)
{
	int max_iters = 100;
	int iters = 0;
	Player player_copy = *this;
	const float skin_factor = 0.05f;
	player_copy.capsule.r += skin_factor;
	while (iters++ < 100)
	{
		int hit_something = 0;
		for (int i = 0; i < colliders.size(); ++i)
		{
			int x = i % width;
			int y = i / width;
			int id = colliders.at(i)->actas;
			if (id > 0) {

				c2Manifold m;
				c2Collide(&colliders.at(i)->collider, 0, tileconv(id), &player_copy.capsule, 0, C2_TYPE_CAPSULE, &m);
				if (m.count) {
					hit_something = 1;
					v2 n = c2(m.n);
					const float corrective_factor = 0.2f;
					player_copy.position += n * corrective_factor;
					player_sync_geometry(&player_copy);
				}
			}
		}
		if (!hit_something) break;
	}
	if (iters == max_iters) printf("NGS failed to converge.\n");
	player_copy.capsule.r = capsule.r;
	player_sync_geometry(&player_copy);
	*this = player_copy;
}


int Player::player_can_fall(int pixels_to_fall,std::vector<block*> colliders, int width)
{
	float min_toi = 1;

	c2AABB player_aabb;
	player_aabb.min = c2(collider.min);
	player_aabb.max = c2(collider.max);

	v2 vel_down_10_pixels = v2(0, (float)-pixels_to_fall);

	for (int i = 0; i < colliders.size(); ++i)
	{
		int x = i % width;
		int y = i / width;
		int id = colliders.at(i)->actas;
		if (id > 0) {
			
			v2 toi_normal;
			v2 toi_contact;
			int iters;
			float toi = c2TOI(&colliders.at(i)->collider, tileconv(id), 0, c2V(0, 0), &player_aabb, C2_TYPE_AABB, 0, c2(vel_down_10_pixels), 1, 0, 0, &iters);
			if (toi < min_toi) {
				min_toi = toi;
			}
		}
	}

	if (min_toi == 1.0f) {
		return 1;
	} else {
		return 0;
	}
}




//TODO: move this to a better place, or preferably, player editable?
C2_TYPE Player::tileconv(int id)
{
	switch (id)
	{
        default: return C2_TYPE_AABB;
        case 0: return C2_TYPE_NONE;
	}
}