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
	util::drawTexture(render, sprite, viewx+(x-8), viewy+(y), 0, 1.0, false, SDL_FLIP_NONE,poses[pose].at((frametime/50)%poses[pose].size()) * 32, 0, 32, 32);
    SDL_Rect splashbox = { viewx + (collider.min.x), viewy + (collider.min.y), 16, 32 };
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderDrawRect(render, &splashbox);
    SDL_Rect orther = { viewx + (lastbox.min.x), viewy + (lastbox.min.y), 16, 32 };
    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
    SDL_RenderDrawRect(render, &orther);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
}
void Player::preStep(double deltaTime) {
    frametime += deltaTime;
    //std::cout << "(" << x << "," << y << ")\n";
    const Uint8* state = SDL_GetKeyboardState(NULL);

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

void Player::moveY(std::vector<block*> colliders, int width, double deltaTime) {
    collider.min = c2V(x, y);
    collider.max = c2V(x + 16, y + 16);
    for (int i = 0; i < colliders.size(); i++) {
        block* blk = colliders.at(i);
        if (blk->actas > 0) {
            double ye = y + (deltaTime / 5 * yvelocity); //y at the end of the frame
            c2AABB box;
            box.min = c2V(x, ye);
            box.max = c2V(x + 16, ye + (16));
            lastbox = box;

            //std::cout << "{" + std::to_string(box.max.x) + ", " + std::to_string(box.max.y) + "}\n";
            if (c2AABBtoAABB(box, blk->collider)) {

                if (yvelocity < 0) {
                    y = blk->y + blk->height;
                    std::cout << "goin down\n";
                    yvelocity = 1;
                }
                else if (yvelocity > 0) {
                    std::cout << "INSIDE!\n";
                    y = blk->y - blk->height;
                    yvelocity = 0;
                    onground = true;
                }
            }
            else {
                if (yvelocity != 0 && !onground) {
                    yvelocity += deltaTime / 50000;

                }
            }

        }
    }
}

void Player::moveX(std::vector<block*> colliders, int width, double deltaTime) {
    collider.min = c2V(x, y);
    collider.max = c2V(x + 16, y + 16);
    for (int i = 0; i < colliders.size(); i++) {
        block* blk = colliders.at(i);
        if (blk->actas > 0) {
            double xe = x + (deltaTime / 5 * xvelocity); //y at the end of the frame
            c2AABB box;
            box.min = c2V(xe, y);
            box.max = c2V(xe + 16, y + (16));
            lastbox = box;

            //std::cout << "{" + std::to_string(box.max.x) + ", " + std::to_string(box.max.y) + "}\n";
            if (c2AABBtoAABB(box, blk->collider)) {

                if (xvelocity < 0) {
                    x = blk->x + blk->height;
                    xvelocity = 0;
                }
                else if (xvelocity > 0) {
                    x = blk->x - blk->height;
                    xvelocity = 0;
                }
            }
            else {
                if (xvelocity != 0) {
                    xvelocity += deltaTime / 50000;
                }
            }

        }
    }

}
void Player::postStep(double deltaTime) {
    if (xvelocity != 0) {
        pose = 1;
    }
    else if (yvelocity > 0) {
        pose = 3;
    }
    else if (yvelocity < 0) {
        pose = 2;
    }
    else {
        pose = 0;
    }
    x = x + deltaTime / 5 * xvelocity;
    y = y + deltaTime / 5 * yvelocity;
    
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
void Player::keyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_z: {
            yvelocity = -2;
            onground = false;
        }
    }
}