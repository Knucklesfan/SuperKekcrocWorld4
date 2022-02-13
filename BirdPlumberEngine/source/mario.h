#include "level.h"
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
    bool freezesprites = false;
    bool hide = false;
    bool onGround = false;
    bool swim = false;
    bool oldswim = false;
    bool sliding = false;
    level* lvl;
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
    bool oldy = false;
    bool ypressed;
    //input is handled differently in SKW4 than JFKMW, so we dont have to worry about
    //implementing SMW styled input. I do not have the same goal as their team.
    //EDIT: I take this back.
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
            respawn(); //this is temporary
            //TODO: handle what to do when the player dies
        }
        if(deathtime > 0) { // if we still gotta pay the time for the crime
            deathtime--;
            if(!deathtime && y > lvl->height-14) { //if the player STILL isn't freaking dead yet
                vsp = Calculate_Speed(1280.0); //step on the gas baby
            }
        } 
        if(!deathtime) {
            vsp = Calculate_Speed(48.0);
            vsp = std::max(-5.0, vsp);
        }
        else {
            hsp = 0.0;
            vsp = 0.0;
        }
    
        //TODO: implement this
        return 1;
    
    }

    //TODO: Implement grabbing



    bool move(double movex, double movey, bool change = false) {
        double newposx = x + movex;
        double newposy = y + movey;
        bool willreturn = true;
        int blockposx = int(newposx/16);
        int blockposy = int(newposy/16);

        int startx = blockposx - CHECKRANGE;
        int starty = blockposy - CHECKRANGE;

        if (startx < 0) {
            startx = 0;
        }
        if (starty < 0) {
            starty = 0;
        }

        //in the original code im basing this on, this entire second is dedicated to sprites
        //there's like 300 lines of sprite code
        //although i find their sprite code really good, and would like to implement it myself
        //i think that i will be better off handling sprites myself
        //i might look there for a reference but outside of that, i think i'm better off
        //writing my own for the time being
        //TODO: implement sprite code

        for (int xrange = startx; xrange < blockposx + CHECKRANGE; xrange++) {
            for (int yrange = startx; yrange < blockposy + CHECKRANGE + (playerstate > 0); xrange++) {
                //id just like to say, as a java developer for years, i love c++
                //and i can finally say this
                //i've gotten to that point wherein i love this language
                //:)
                double f_h = getfloory(newposx + 8.0 - (blockposx * 16), blockposx, blockposy);
                double f_w = getfloorw(newposx, blockposy);
                double BelowBlock = double(blockposy * 16) + (f_h - 16.0) - (height)-1;
                double AboveBlock = double(blockposy * 16) + (f_h)-1;
                double RightBlock = double(blockposx * 16) + f_w;
                double LeftBlock = double(blockposx * 16) - f_w;
                //TODO: figure out how this applies
                //im most likely gonna have to wait until i can actually get 
                //this in game to figure out how these work, 
                //but still.
                bool checkLeft = true;
                bool checkRight = true;
                bool checkBottom = true;
                bool checkTop = true;

                bool playHitSound = true;

                if (swim && movey > 0.0) {
                    if (lvl->gettile(blockposx, blockposy, 1) == 0) {
                        if (pad[button_up] && (pad[button_b] || pad[button_a])) {
                            vsp = Calculate_Speed(0x500+ (playerstate > 0) * 0x80);
                            spinjumping = pad[button_a];

                        }
                        else {
                            checkBottom = true;
                            playHitSound = false;
                        }
                    }
                }
                if (newposx < RightBlock && newposx > LeftBlock && newposy < AboveBlock && newposy > BelowBlock) {
                    //TODO: handle hurt blocks

                    //TODO: handle block actions, right now
                    //blocks can't be interacted with, nor can they
                    //interact with the player.
                    if (movex < 0.0 && checkRight) {
                        if (newposx > RightBlock - boundsx) {
                            newposx = RightBlock;
                            willreturn = false;
                            
                            //TODO: block handling here too
                        }
                    }
                    if (movex > 0.0 && checkLeft) {
                        if (newposx < LeftBlock+ boundsx) {
                            newposx = LeftBlock;
                            willreturn = false;

                            //TODO: block handling here too
                        }
                    }
                    if (movey < 0.0 && checkTop) {
                        double ybound = boundsy;
                        int slope = get_slope(blockposx, blockposy);
                        if (slope != 0) {
                            ybound += 2;
                        }
                        if (newposy > AboveBlock - ybound) {
                            willreturn = false;

                            if (slope != 0 && slopetype == 0) {
                                slopetype = slope;
                            }

                            if (vsp <= 0) {
                                newposy = AboveBlock;
                            }

                            if ((spinjumping && lvl->gettile(blockposx, blockposy, 1)->actas == 0x11E) && playerstate > 0) { //TODO: change this number as well!
                                //handle block processing
                                vsp = Calculate_Speed(768);
                                willreturn = true;
                            }
                            else {
                                //handle block processing
                            }
                            if (pad[button_down]) {
                                //ALLLL of this is pipe related
                                //TODO: implement pipes
                                //yeah, i know i said i didnt want pipes but uh
                                //its just gonna be easier for the player tbh
                                //I want pipes in the game, is that all it takes???

                            }
                        }
                    }
                    if (movey > 0.0 && checkBottom) {
                        if (newposy < BelowBlock + boundsy) {
                            newposy = BelowBlock;
                            willreturn = false;
                            if (playHitSound) {
                                //PLAY A SOUND EFFECT
                                //TODO: add sound effect to game
                                //(this is easy stuff)

                            }
                            //once more, block processing needs to be done. we're waiting, knux
                            if (pad[button_up]) {
                                //MORE PIPE STUFF, WTF
                                //TODO: pipes
                            }
                        }
                    }
                    //TODO: grabbing stuff too :(
                }
            }
        }
        x = newposx;
        y = newposy;
        return willreturn;

    }
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

    int logic() {
        if (freezesprites) {
            if (dead) {
                return deathlogic();
            }
            return 1;
        }
        slopetype = 0;

        getinput();
        if (pad[button_y] != oldy) {
            oldy = pad[button_y];
            if (oldy) {
                ypressed = true;
            }
        }

        height = (playerstate > 0 && crouch == 0) ? 28.0 : 14.0;

        if (y < -32.0 && !dead) { //IF YOU FIND A BUG, CHANGE THE Y NUMBER HERE!
            die();
        }
        if (dead) {
            return deathlogic();
        }
        if (invinc_frames > 0) {
            invinc_frames--;
        }
        hide = invinc_frames > 0 ? !hide : false; //alright, ngl, i picked a bad name
        //YES, HIDE MAKES YOU INVISIBLE. STOP ASKING.

        double gravity = -double(GRAVITY);
        bool running = false;
        bool moving = false;
        bool shs = false; //SLIGHT HIGH SPEED.. whatever that means

        int checkx1 = int((x + 8) / 16.0);
        int checky1 = int((y + height) / 16.0);

        swim = lvl->gettile(checkx1, checky1, 1)->actas < 4;
        if (swim != oldswim) {
            oldswim = swim;
            if (swim) {
                vsp = 0;
                hsp = 0;
                pmeter = 0;

                //TODO: particles
            }
        }
        //TODO: pipes
        if (false) { // replace with pipes plz

        }
        else {
            if (!swim) {
                if (onGround) { //clearing stuff
                    cansprint = false;
                }

                onGround = false; 
                if (!move(0.0, -1.0, true)) { //checks for floors
                    if (vsp <= 0) {
                        onGround = true;
                        spinjumping = false;
                    }
                    else {
                        y += 1;
                    }
                }
                else { //no floor no pinecones
                    y += 1; //lol get dropped
                }
                skidding = 0;
                if (abs(hsp) >= Calculate_Speed(576.0)) {
                    shs = true; //lol i dont know what this means
                    //its 12:03 am
                    // i wanna bed 
                    //TODO: go to bed

                    if (pad[button_y] && (pad[button_left] || pad[button_right])) {
                        if (cansprint || (onGround && !crouch)) {
                            pmeter += 2;
                            if (pmeter > P_METER_REQUIRED) {
                                pmeter = P_METER_REQUIRED;
                            }
                        }
                        else {
                            if (pmeter > 0) {
                                pmeter--;
                            }
                        }
                    }
                }
                else {
                    if (pmeter > 0) {
                        pmeter--;
                    }
                }
                if (pmeter >= (P_METER_REQUIRED - 1) && pad[button_y]) {
                    cansprint = true;
                }
                if (pad[button_left]) {
                    walk_dir = -1;
                    moving = true;
                }
                if (pad[button_right]) {
                    walk_dir = 1;
                    moving = true;

                }
                //this is code for slide cancelling

                if (sliding) {
                    if ((moving == true || (hsp == 0 && !slopetype)) && onGround) {
                        sliding = false;
                    }
                }
                if (pad[button_down] && !sliding) { //if we arent sliding, but we wanna slide
                    if (slopetype != 0) {
                        //handle grabbing stuff
                        if (moving == false && !(pad[button_left] || pad[button_right])) {
                            sliding = true;
                        }
                        else {
                            crouch = false;
                        }
                    }
                    else {
                        if (onGround) {
                            walk_dir = 0;
                            moving = true;
                            crouch = true;
                        }
                    }
                }
                else {
                    if (onGround) {
                        crouch = false;
                    }
                }
                if (pad[button_y]) {
                    running = true;
                }

                if ((pad[button_a] || pad[button_b]) != lastjumped) {
                    lastjumped = pad[button_a] || pad[button_b];
                    if (lastjumped && onGround) {
                        if (pad[button_a])
                        {
                            sliding = false;
                            //Spinjump
                            vsp = Calculate_Speed(1136.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                            //TODO: play sound here
                            spinjumping = true;
                        }
                        else
                        {
                            sliding = false;
                            //Normal jump
                            vsp = Calculate_Speed(1232.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                            //TODO: play sound here

                            //jump_is_spin = false;
                        }
                    }
                }
                /*
                okay, so since it's late, and ive been up all night, this is where
                i call it quits for the night. Current state of code:
                * got a little bit of the logic working, but not much yet
                * need to implement player physics.
                * currently on line 1135 of player movement code
                to people from the future reading this: Hello!
                I really hope this game is a success, and I really want to tell everyone
                that I send my regards, and hope you enjoy kekcroc world 4
                i mean
                its not really about that, since yknow
                kekcroc world 4 is just one of the games im making this engine for
                but i really hope this engine provides a lot of people with fun times
                for years to come.
                
                */
            }
        }

    }





    //THESE NUMBERS **WILL** CHANGE
    //why am i using SMW tiles instead of enums? easy deployment, for the time being.
    //enums ARE coming, and tiled support is the plan.
    //why, might you ask, am i using a hyper realisitc smw engine to base this off of?
    //guess i just hate myself lmao


    double getfloory(double truex, int blockx, int blocky) {
        int tileid = lvl->gettile(blockx, blocky, 1)->actas;
        if (tileid == 0x1AA || tileid == 0x1AB) //45* slope Right
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return truex;
        }
        if (tileid == 0x1AF || tileid == 0x1B0) //45* slope Left
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return 16.0 - truex;
        }
        if (tileid == 0x196) //23* slope Right P1
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return truex / 2;
        }
        if (tileid == 0x19B) //23* slope Right P2
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return 8.0 + truex / 2;
        }
        if (tileid == 0x1A0) //23* slope Left P1
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return 16.0 - truex / 2;
        }
        if (tileid == 0x1A5) //23* slope Left P2
        {
            if (truex < 0 || truex > 16) {
                return -9999; 
            }
            return 8.0 - truex / 2;
        }
        return 16.0;
    }
    double getfloorw(int blockx, int blocky) { 

        int tileid = lvl->gettile(blockx, blocky, 1)->actas;
        if ((tileid == 0x1AA || tileid == 0x1AB) || (tileid == 0x1AF || tileid == 0x1B0)) {
            return 16.0;
        }
        if ((tileid == 0x1A0 || tileid == 0x1A5) || (tileid == 0x196 || tileid == 0x19B)) {
            return 16.0;
        }
        return 15.0;
    }

    //i lifted this straight from jfkmw, but that's okay because
    //i completely understand how this works
    //also, all of these numbers WILL change whenever i implement slopes,
    //just havent gotten around to that yet, okay?
    int get_slope(int blockx, int blocky)
    {
        int tileid = lvl->gettile(blockx, blocky, 1)->actas;
        /*
            45
        */
        if (tileid == 0x1AA || tileid == 0x1AB) { return 1; }
        if (tileid == 0x1AF || tileid == 0x1B0) { return 2; }
        /*
            23
        */

        if (tileid == 0x196) { return 3; }
        if (tileid == 0x19B) { return 4; }

        if (tileid == 0x1A0) { return 5; }
        if (tileid == 0x1A5) { return 6; }
        return 0;
    }






};
