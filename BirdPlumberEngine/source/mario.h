#pragma once
#include <SDL2/SDL.h>

#define CHECKRANGE 2
#define boundsx 6.0 
#define boundsy 6.0
#define button_b 0
#define button_y 1
#define button_a 2
#define button_left 3
#define button_right 4
#define button_up 5
#define button_down 6
#define button_start 7
#define GRAVITY 96
#define P_METER_REQUIRED 0x70
#define marmin(a,b)            (((a) < (b)) ? (a) : (b))
#define marmax(a,b)            (((a) > (b)) ? (a) : (b))

enum slope {
    SLOPEHIGHR = 1,
    SLOPEHIGHL = 2,
};
#define Calculate_Speed(x) double(x) / 256.0 //didnt even know you could do this before writing this, 
//but im gonna abuse the crap out of this functionality of cpp defines
class level;
class mplayer {

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
    bool freezesprites = false;
    bool hide = false;
    bool onGround = false;
    bool swim = false;
    bool oldswim = false;
    bool sliding = false;
    bool climbing = false;
    unsigned int slopetype = 0;
    /*
    0 : None
    1 : Right 45*
    2 : Left 45*
    */


    int toscale = 1;
    unsigned int pmeter = 0;

    double hsp = 0.0;
    double vsp = 0.0;
    double vanillax = 0; //this is the equivalent to 0x7B https://smwc.me/m/smw/ram/7E007B/
    double vanillay = 0; //this is the equivalent to 0x7D https://smwc.me/m/smw/ram/7E007D/
    
    double camerax = 0; //the X position of the camera, in the player's view.
    double cameray = 0; //the y position of the camera, in the player's view.
    int xcam = 0; //this is the equivalent to 0x7E1411 https://smwc.me/m/smw/ram/7E1411/
    int ycam = 0; //this is the equivalent to 0x7E1412 https://smwc.me/m/smw/ram/7E1411/
    double smooth_camera_speed = 5; //TODO: tweak this number!


    double prvious_hsp = 0.0; //this is the last x speed, stored for whatever reason we need.
    double prvious_vsp = 0.0; //this is the last y speed, stored for whatever reason we need.

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
    bool oldy = false;
    bool ypressed;
    //input is handled differently in SKW4 than JFKMW, so we dont have to worry about
    //implementing SMW styled input. I do not have the same goal as their team.
    //EDIT: I take this back.
    bool lastjumped = false;
    bool cansprint = false;
    bool smoothcam = true;
    level* lvl;
    mplayer(level* lev, double newx = 0.0, double newy = 0.0);

    void die();

    //returns true if attack was vital enough to kill player, otherwise returns false.

    bool hurt();
    void respawn();
    void getinput();
    void stomp();
    int deathlogic();
    void render(SDL_Renderer* render);
    //TODO: Implement grabbing



    bool move(double movex, double movey, bool change = false);
    //now, this code is commented mainly because rendering isn't a focus right now.
    //it's 11:35pm, i just want to get the barebones down to finish this tomorrow.

    /*
    * 
    * 
    * 
    * explaination for this code being present, btw.
    * I'm gonna have to basically redo all of this
    * I don't like how JFKMW does it's graphics, in fact i find their implementation
    * of the graphics code a bit ancient and barbaric, alongside
    * a slog to work with. Rather, i'd rather render my graphics
    * using standard SDL2 sprites, instead of palettes.
    * now, is this gonna matter to the general player? no,
    * BUT it means that creators have more freedom, and this is mainly
    * a long term decision.
    * ...but that's coming from the developer who currently has only BMP+OGG support
    * implemented, so i dunno man.
    * 
    * 
    	void Get_Sprite()
	{
		if (DEAD) {
			sprite = "DEAD";
			return;
		}
		string NewSprite = "STAND";
		if (pad[button_up]) {
			NewSprite = "UP";
		}
		if (GRABBED_SPRITE != 0xFF) {
			if (NewSprite == "STAND") {
				NewSprite = "RUN0";
			}
			NewSprite = "GRAB_" + NewSprite;
		}
		if (jump_is_spin && !ON_FL) {
			switch ((global_frame_counter / 2) % 4) {
			case 0:
				NewSprite = "PIPE";
				to_scale = 1;
				break;
			case 1:
				NewSprite = "STAND";
				to_scale = 1;
				break;
			case 2:
				NewSprite = "BACK";
				to_scale = 1;
				break;
			case 3:
				NewSprite = "STAND";
				to_scale = -1;
				break;
			}

			sprite = NewSprite + "_" + to_string(STATE);
			return;
		}
		if (in_pipe) {
			sprite = "PIPE_" + to_string(STATE);
			return;
		}
		if (SLIDING) {
			sprite = "SLIDE_" + to_string(STATE);
			return;
		}
		if (!CROUCH) {
			if (!ON_FL) {
				if (IN_WT) {
					NewSprite = "SWIM";
				}
				else {
					if (GRABBED_SPRITE != 0xFF)
					{
						NewSprite = "GRAB_RUN" + to_string(1 + STATE);
					}
					else
					{
						if (CAN_SPRINT) {
							NewSprite = "JUMPB";
						}
						else {
							if (Y_SPEED > 0.0) {
								NewSprite = "JUMP";
							}
							else {
								NewSprite = "FALL";
							}
						}
					}
				}
			}
			else {
				if (SKIDDING == 0) {
					if (X_SPEED != 0) {
						FRM += X_SPEED / 5;
						int Frame = abs(int(FRM) % (2 + (STATE > 0)));
						if (GRABBED_SPRITE != 0xFF)
						{
							NewSprite = "GRAB_RUN" + to_string(Frame);
						}
						else
						{
							if (CAN_SPRINT) {
								NewSprite = "RUN" + to_string(Frame);
							}
							else {
								NewSprite = "WALK" + to_string(Frame);
							}
						}
					}
				}
				else {
					NewSprite = GRABBED_SPRITE == 0xFF ? "SKID" : "PIPE";
				}
			}
		}
		else {
			if (GRABBED_SPRITE == 0xFF) {
				NewSprite = "CROUCH";
			}
			else {
				NewSprite = "GRAB_CROUCH";
			}
		}
		if (WALKING_DIR > 0)
		{
			to_scale = -1;
		}
		if (WALKING_DIR < 0)
		{
			to_scale = 1;
		}
		sprite = NewSprite + "_" + to_string(STATE);
	}

    
    
    
    
    
    
    
    
    
    
    
    
    
    */

    //TODO: pipes, just... pipes in general. please implement pipes!

    //

    int logic();
    void cameralogic();
    int process();





    //THESE NUMBERS **WILL** CHANGE
    //why am i using SMW tiles instead of enums? easy deployment, for the time being.
    //enums ARE coming, and tiled support is the plan.
    //why, might you ask, am i using a hyper realisitc smw engine to base this off of?
    //guess i just hate myself lmao


    double getfloory(double truex, int blockx, int blocky);
    double getfloorw(int blockx, int blocky);

    //i lifted this straight from jfkmw, but that's okay because
    //i completely understand how this works
    //also, all of these numbers WILL change whenever i implement slopes,
    //just havent gotten around to that yet, okay?
    int get_slope(int blockx, int blocky);






};
