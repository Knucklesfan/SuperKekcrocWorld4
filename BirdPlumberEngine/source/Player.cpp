#include "Player.h"

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
    std::cout << "(" << x << "," << y << ")\n";
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        y -= 1;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        y += 1;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        x += 1;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        x -= 1;
    }

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