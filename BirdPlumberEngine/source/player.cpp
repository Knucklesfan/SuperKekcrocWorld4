#include "player.h"

Player::Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render) {
    type = object::PLAYER;
	x = xa;
	y = ya;
	sprite = sheet;
    SDL_Surface* surf = SDL_LoadBMP("./players/kekcroc/0.bmp");
    sprite = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
}
void Player::render(SDL_Renderer* render, int viewx, int viewy) {
	util::drawTexture(sprite, viewx+x, viewy+y, 0, 1.0, false, SDL_FLIP_NONE, render);
}
void Player::logic(double deltaTime) {
    //std::cout << "(" << x << "," << y << ")\n";
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        yvelocity = -1;
    }
    else if (state[SDL_SCANCODE_DOWN]) {
        yvelocity = 1;
    }
    else {
        yvelocity -= deltaTime/500;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        xvelocity = 1;
    }
    else if (state[SDL_SCANCODE_LEFT]) {
        xvelocity = -1;
    }
    else {
        xvelocity = 0;
    }
}

void Player::physics(double deltatime, std::vector<block*> colliders, int width, std::vector<GameObject*> objects) {
    
    c2AABB c;
    c.min = c2V(floor(x),floor(y));
    c.max = c2V(floor(x)+16, floor(y)+32);

    for (block* blocks : colliders)
    {
        if (blocks->actas != 0) {
            c2Manifold m;
            c2AABBtoAABBManifold(c, blocks->collider, &m);
            if (m.depths[0] > 0 )
            {
                int hit = m.depths[0];
                if (hit > blocks->width / 2) {
                    hit = -((int)(m.depths[0]) % blocks->width / 2);
                }
                if (hit > 0) {
                    if (xvelocity > 0) {
                        x -= (double(deltatime / 5) * xvelocity);
                        xvelocity = 0;
                    }
                    else {
                        x += (double(deltatime / 5) * xvelocity);
                        xvelocity = 0;
                    }
                }
                std::cout << "Contact X:" << hit << " Y: " << m.depths[1] << "\n";
            }
        }
    }
    x += (double(deltatime/5)*xvelocity);
}
int Player::getx() {
    return x;
}

int Player::gety() {
    return y;
}

int Player::gettype() {
    return type;
}