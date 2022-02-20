#pragma once

#define COINSIZE 16

#include "GameObject.h"
#include "util.h"
#include "block.h"
class bullet :
    public GameObject
{
public:
    double frametime = 0;
    bool exploding = false;
    float explodetimer = 0;
    int hsp = 0;
    bullet() {};
    bullet(int x, int y, SDL_Texture* text, int dir) {
        bullet::x = x;
        bullet::y = y;
        bullet::sprite = text;
        active = true;
        exploding = false;
        hsp = dir;
    };
    void render(SDL_Renderer* render, int viewx, int viewy) {
        if (active) {
            std::cout << "drawin n livin\n";
            util::drawTexture(render, sprite, viewx + (x), viewy + (y), 0, 1.0, false, SDL_FLIP_NONE, !exploding?0:(((int)explodetimer%3)+1)*16, 0, 16, 16);
        }
    };
    void logic(double deltaTime) {
        if (!exploding) {
            x += deltaTime * hsp * 500;
        }
        else {
            if (explodetimer < 5) {
                explodetimer += deltaTime * 10;
            }
            else {
                active = false;
                exploding = false;
            }

        }
        
    };
    void phys(double deltaTime, std::vector<block*> colliders, int width) {
        if (checkForCollision(x, y, 16, 16, colliders, 1, width)) {
            exploding = true;
        }
    };
    void collided(GameObject* object, level* lvl) {

    };
    int getx() { return x; }
    int gety() { return y; }
    int gettype() { return type; }
    bool getactive() { return active; }

    int checkForCollision(int x, int y, int w, int h, std::vector<block*> colliders, int id, int width) {

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

};

