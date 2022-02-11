#include "level.h"

#define button_b 0
#define button_y 1
#define button_a 2
#define button_left 3
#define button_right 4
#define button_up 5
#define button_down 6
#define button_start 7

#define Calculate_Speed(x) double(x) / 256.0 //didnt even know you could do this before writing this, 
//but im gonna abuse the crap out of this functionality of cpp defines

class player {

    //ill be completely honest here: this is my translation of JFKMW's mario movement system
    //alongside my own interpretations of the SMW source and attempts to gain compatibility
    //with what i've already got here with Tiled, and the levels system as a whole. 
    //this entire project is a whole WIP, but if i can get it to work
    //then this'll be extremely valuable.
    //The git repo I found this code from (which I assume is the original, as the original
    //just disappeared one day without a trace?) says this is under the

    public:

    int skidding = 0;
    int walk_dir = 0;
    bool crouch = false;
    int invinc_frames = 0;
    
    bool hide = false;
    bool onGround = false;
    bool swim = false;
    bool sliding = false;
    level* lvl;
    unsigned int slope = 0;
    int toscale = 1;
    unsigned int pmeter = 0;

    double hsp = 0.0;
    double vsp = 0.0;

    double prvious_hsp = 0.0; //this is the last x speed, stored for whatever reason we need.

    double height = 14.0;

    double x = 0.0;
    double y = 0.0;

    unsigned int playerstate = 0;
    unsigned int playerindex = 0;

    //TODO: grabbing needs implementation. I'm not doing this today
    
    bool dead = false;
    bool active = false;

    bool spinjumping = false;

    //TODO: Potentially implement pipes? not sure if i want to do so.

    bool runbutton = false;

    unsigned int deathtime = 0;

    bool controllable = false;
	bool pad[7] = { false,false,false,false,false,false,false };

    //input is handled differently in SKW4 than JFKMW, so we dont have to worry about
    //implementing SMW styled input. I do not have the same goal as their team.

    bool lastjumped = false;
    bool cansprint = false;

    player(level* currlvl, double newx=0.0, double newy=0.0) {
        x=newx;
        y=newy;
        lvl = currlvl;

    }

    void die() {
        if(controllable && !dead) {
            deathtime = y < -16.0 ? 150 : 32;
            dead = true;
        }
    }

    //returns true if attack was vital enough to kill player, otherwise returns false.

    bool hurt() {
        if(!dead && invinc_frames <= 0) {
            if(playerstate != 0) {
                playerstate = 0;
                invinc_frames = 90;
            }
            else {
                die();
                return true;
            }
        }
        return false;
    }
    void respawn() {
        x = lvl->startx;
        y = lvl->starty;

        deathtime = 0;
        dead = false;
        playerstate = 0;
        hsp = 0;
        vsp = 0;
        spinjumping = false;
        active = true;
    }
    void getinput() {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(controllable) {
            pad[button_y] = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_X];
            pad[button_b] = state[SDL_SCANCODE_Z];
            pad[button_a] = state[SDL_SCANCODE_X];
            pad[button_left] = state[SDL_SCANCODE_LEFT];
            pad[button_right] = state[SDL_SCANCODE_RIGHT];
            pad[button_up] = state[SDL_SCANCODE_UP];
            pad[button_down] = state[SDL_SCANCODE_DOWN];

        }
    }
    void stomp() {
        //TODO: implement particles

        if(!spinjumping) {
            if(pad[button_b] || pad[button_a]) {
                vsp = Calculate_Speed(1408); //I HAVE NO IDEA WHERE THESE NUMBERS COME FROM
            }
            else {
                vsp = Calculate_Speed(800); //TODO: find where these numbers come from
            }
        } //looking at the original, I don't really see what the difference is between
        //enemy jump and enemy jump spin. This might come up later, though, so i should
        //be hesistant moving foward.


        //...foward
        //TODO: learn how to spell

    }
    int deathlogic() {
        x += hsp;
        y += vsp;

        if(y < lvl->height-100) {
            //TODO: handle what to do when the player dies
        }
        if(deathtime > 0) { // if we still gotta pay the time for the crime
            deathtime--;
            if(!deathtime && y > lvl->height-14) { //if the player STILL isn't freaking dead yet
                vsp = Calculate_Speed(1280.0); //step on the gas baby
            }
        } 
        if(!deathtime) {

        }
    
    
    }


};