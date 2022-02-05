#pragma once

#include "GameObject.h"
#include "util.h"
#include "message.h"
#define COINSIZE 16 //mandatory square size because im evil hahahaha

//PLEASE DONT CANCEL ME ON TWITTER, THAT'S GONNA CHANGE IN LATER UPDATES I PROMISE


class coin :
    public GameObject
{
    public:
        double frametime = 0;
        coin() {};
        coin(int x, int y, SDL_Texture* text);
        void render(SDL_Renderer* render, int viewx, int viewy);
        void logic(double deltaTime);
        void phys(double deltaTime, std::vector<int> colliders) {};
        void collided(GameObject* object, level* lvl);
        int getx() { return x; }
        int gety() { return y; }
        int gettype() { return type; }
        bool getactive() { return active; }

};

