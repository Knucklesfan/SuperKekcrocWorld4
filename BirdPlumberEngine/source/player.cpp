#include "player.h"
#include "level.h"
Player::Player(int xa, int ya, SDL_Texture* sheet, SDL_Renderer* render) {
    type = object::PLAYER;
    x = xa;
    y = ya;
    sprite = sheet;
    SDL_Surface* surf = SDL_LoadBMP("./players/kekcroc/0.bmp");
    sprite = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
    prevstate = SDL_GetKeyboardState(NULL);
}
void Player::render(SDL_Renderer* render, int viewx, int viewy) {
    util::drawTexture(render, sprite, viewx + (x+(facing*32))-32, viewy + (y), 0, 1.0, false, facing ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL, poses[pose].at((frametime / posespeeds[pose]) % poses[pose].size()) * 48, 0, 48, 32);
}
void Player::preStep(double deltaTime) {
    frametime += 4 + (abs(hsp)/hsp_max);

}

void Player::postStep(double deltaTime, std::vector<GameObject*> objs, level* lvl) {
    for (GameObject* obj : objs) {
        if (objectcollision(x, y, 14, 30, obj)) {
            obj->collided(nullptr, lvl);
        }
    }
    pose = 0;

    if (hsp > 0) {
        facing = true;
        pose = 1;
    }
    else if (hsp < 0) {
        facing = false;
        pose = 1;
    }

    if (vsp > 0) {
        pose = 3;
    }
    else if (vsp < 0) {
        pose = 2;
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_X] != prevstate[SDL_SCANCODE_X] || state[SDL_SCANCODE_X]) {
        //shoot a bullet
        lvl->instance(new bullet(x+16*(facing?1:-1), y+8, lvl->textures[1], (facing ? 1 : -1)));
        std::cout << "fire!\n";
    }
    prevstate = state;
}

void Player::durangoController(std::vector<block*> colliders, int width, double deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    int kright, kleft, kjump, xdir, krun;
    kright = state[SDL_SCANCODE_RIGHT];
    kleft = state[SDL_SCANCODE_LEFT];
    kjump = state[SDL_SCANCODE_Z];
    xdir = kright;
    if (kleft) {
        xdir = -1;
    }

    onground = checkForCollision(x, y + 1, 14, 30, colliders, 1, width);
    krun = state[SDL_SCANCODE_A];
    if (xdir != 0)
    {

        hsp = approach(hsp, (hsp_max * (1 + krun)) * xdir, accel);
    }
    else
    {
        hsp = approach(hsp, 0, fric);
    }

    if (kjump)
    {
        if (onground) {
            vsp -= (vsp_max);
        }
        if (jumpmod < 5) {
            jumpmod += 0.5;
        }
    }
    else {
        jumpmod = 0;
    }
    //std::cout << "KEYLEFT: " << kleft << " KEYRIGHT: " << kright << " XDIR: " << xdir << " HSP: " << hsp << "VSP: " << vsp << "\n";
    vsp -= jumpmod;
    if (!onground)
    {
        vsp = approach(vsp, vsp_max, grav);
    }
    double dy;
    for (int i = 0; i < abs(hsp); i++) {
        if (onground) {

            if (checkForCollision(x + sign(hsp) * deltaTime, y, 14, 30, colliders, 1, width)) {
                dy = calcSlopeUp(hsp * deltaTime, colliders, width);     // Measure the slope
                if (dy <= slope_max) // Climbable
                {
                    x += sign(hsp) * deltaTime;
                    y -= dy * deltaTime;
                    continue;
                }
                hsp = 0; // Unclimbable
                break;
            }
            else if (!checkForCollision(x + sign(hsp) * deltaTime, y + 1, 14, 30, colliders, 1, width)) // Down slope (just like we did up slopes)
            {
                dy = calcSlopeDown(hsp * deltaTime, colliders,width);
                if (dy <= slope_max)
                {
                    x += sign(hsp) * deltaTime;
                    y += dy * deltaTime;
                    continue;
                }
                x += sign(hsp) * deltaTime; // Continue the loop rather than break the loop to keep momentum
                continue;
            }
            else { x += sign(hsp) * deltaTime; } // Flat
        }
        else // We're in the air so we don't check for slopes
        {
            if (!checkForCollision(x + sign(hsp) * deltaTime, y, 14, 30, colliders, 1, width)) x += sign(hsp) * deltaTime;
            else
            {
                hsp = 0;
                break;
            }
        }
    }
    for (int i = 0; i < std::abs(vsp); i++) // Vertical movement
    {
        if (!checkForCollision(x, y + sign(vsp) * deltaTime, 14, 30, colliders, 1, width)) {
            y += sign(vsp) * deltaTime;
        }
        else
        {
            vsp = 0;
            break;
        }
    }

    if (checkForCollision(x, y, 14, 30, colliders, 1, width)) {
        //Right
        if (!checkForCollision(x + 1, y, 14, 30, colliders, 1, width)) {
            x += 1;
        }
        //Left
        if (!checkForCollision(x - 1, y, 14, 30, colliders, 1, width)) {
            x -= 1;
        }
        //Up
        if (!checkForCollision(x, y - 1, 14, 30, colliders, 1, width)) {
            y -= 1;
        }
        if (!checkForCollision(x, y + 1, 14, 30, colliders, 1, width)) {
            y += 1;
        }
    }
}

void Player::keyPress(SDL_Keycode key) {

}
//checks for collisions between objects at a point. If id = -1, then it checks for any collisions above 0, and returns the int of the first one to check the box, or -1 if nothing.

int Player::checkForCollision(int x, int y, int w, int h, std::vector<block*> colliders, int id, int width) {

        c2AABB box;
        box.min = c2V(x, y);
        box.max = c2V(x + w, y + h);
        for (int tx = x / 16; tx < x / 16 + 2; tx++) {
            for (int ty = y / 16; ty < y / 16 + 2; ty++) {
                if (((ty + 1) * width) + tx > colliders.size()) {
                    return 0;
                }
                block* blk = colliders.at(((ty + 1) * width) + tx);
                //std::cout << "x: " << x / 16 << " y: " << y / 16 << " tx: " << tx << " ty: " << ty << "\n";
                if (id >= 0 ? (blk->actas == id) : (blk->actas > 0)) {
                    if (c2AABBtoAABB(box, blk->collider)) {
                        return 1;
                    }
                }
            }
        }
    return 0;
}

int Player::objectcollision(int x, int y, int w, int h, GameObject* obj) {
    c2AABB box;
    box.min = c2V(x, y);
    box.max = c2V(x + w, y + h);
    if (c2AABBtoAABB(box, obj->box)) {
        return 1;
    }
    return 0;
}
double Player::approach(double start, double end, double shift) {
    if (start < end) return std::min(start + shift, end);
    return std::max(start - shift, end);
}

int Player::sign(int num) {
    return (num > 0) - (num < 0);
}

int Player::calcSlopeUp(double hsp, std::vector<block*> colliders, int width) {
    int dy = 0;
    while (checkForCollision(x + sign(hsp), y - dy, 14, 30, colliders, 1, width)) // While we are not at the top of the slope
    {
        dy++;
        if (dy > slope_max) break; // Break if the slope is greater than we can climb
    }
    return dy;

}

int Player::calcSlopeDown(double hsp, std::vector<block*> colliders, int width) {
    int dy = 0;
    while (!checkForCollision(x + sign(hsp), y + dy + 1, 14, 30, colliders, 1, width)) // While we are not at the bottom of the slope
    {
        dy++;
        if (dy > slope_max) break; // Break if the slope is greater than we can climb
    }
    return dy;
}
