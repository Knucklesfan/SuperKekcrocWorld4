#include "mario.h"
#include "level.h"

mplayer::mplayer(level* currlvl, double newx, double newy) {
    x = newx;
    y = newy;
    lvl = currlvl;
    camerax = x;
    cameray = y;
    deathtime = 0;
    dead = false;
    playerstate = 0;
    hsp = 0;
    vsp = 0;
    spinjumping = false;
    active = true;
    controllable = true;

}

void mplayer::die() {
    if (controllable && !dead) {
        deathtime = y < -16.0 ? 150 : 32;
        dead = true;
    }
}

//returns true if attack was vital enough to kill player, otherwise returns false.

bool mplayer::hurt() {
    if (!dead && invinc_frames <= 0) {
        if (playerstate != 0) {
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
void mplayer::respawn() {
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
void mplayer::getinput() {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (controllable) {
        pad[button_y] = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_S];
        pad[button_b] = state[SDL_SCANCODE_Z];
        pad[button_a] = state[SDL_SCANCODE_X];
        pad[button_left] = state[SDL_SCANCODE_LEFT];
        pad[button_right] = state[SDL_SCANCODE_RIGHT];
        pad[button_up] = state[SDL_SCANCODE_UP];
        pad[button_down] = state[SDL_SCANCODE_DOWN];

    }
}
void mplayer::stomp() {
    //TODO: implement particles

    if (!spinjumping) {
        if (pad[button_b] || pad[button_a]) {
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
int mplayer::deathlogic() {
    x += hsp;
    y += vsp;

    if (y < lvl->height - 100) {
        respawn(); //this is temporary
        //TODO: handle what to do when the player dies
    }
    if (deathtime > 0) { // if we still gotta pay the time for the crime
        deathtime--;
        if (!deathtime && y > lvl->height - 14) { //if the player STILL isn't freaking dead yet
            vsp = Calculate_Speed(1280.0); //step on the gas baby
        }
    }
    if (!deathtime) {
        vsp = Calculate_Speed(48.0);
        vsp = max(-5.0, vsp);
    }
    else {
        hsp = 0.0;
        vsp = 0.0;
    }

    //TODO: implement this
    return 1;

}

//TODO: Implement grabbing


bool mplayer::move(double movex, double movey, bool change) {

    double NewPositionX = x + hsp;
    double NewPositionY = y + vsp;
    bool willreturn = true;
    int_fast16_t PosXBlock = int_fast16_t(NewPositionX / 16);
    int_fast16_t PosYBlock = int_fast16_t(NewPositionY / 16);
    int_fast16_t startX = PosXBlock - CHECKRANGE;
    if (startX < 0)
        startX = 0;
    int_fast16_t startY = PosYBlock - CHECKRANGE;
    if (startY < 0)
        startY = 0;


    /*
    Shitty block collision. Surprised this doesn't lag.
    */
    for (int_fast16_t xB = startX; xB < PosXBlock + CHECKRANGE; xB++)
    {
        for (int_fast16_t yB = startY; yB < PosYBlock + CHECKRANGE + (playerstate > 0); yB++)
        {
            block* block = lvl->gettile(xB, yB, 1);
            bool checkLeft = true;
            bool checkRight = true;
            bool checkBottom = true; //this might be the problem
            bool checkTop = true;
            if (block->actas == 25 || block->actas == 0) {
                checkLeft = false;
                checkRight = false;
                checkBottom = false;
                checkTop = false;
            }
            double f_h = getfloory(NewPositionX + 8.0 - (xB * 16), xB, yB);
            double f_w = getfloorw(xB, yB);
            double BelowBlock = double(yB * 16) + (f_h - 16.0) - (height)-1;
            double AboveBlock = double(yB * 16) + (f_h)-1;
            double RightBlock = double(xB * 16) + f_w;
            double LeftBlock = double(xB * 16) - f_w;


            bool playHitSound = true;

            if (swim && vsp > 0.0)
            {
                if (lvl->gettile(xB, yB,1)->actas == 0)
                {
                    if (pad[button_up] && (pad[button_b] || pad[button_a]))
                    {
                        vsp = Calculate_Speed(0x500 + (playerstate > 0) * 0x80);
                        spinjumping = pad[button_a];

                    }
                    else
                    {
                        checkBottom = true;
                        playHitSound = false;
                    }
                }
            }

            if (NewPositionX < RightBlock && NewPositionX > LeftBlock && NewPositionY < AboveBlock && NewPositionY > BelowBlock)
            {
                /* TODO: hurt blocks!
                if (map16_handler.logic[4])
                {
                    Hurt();
                }
                if (map16_handler.tile == 5)
                {
                    Die();
                }

                map16_handler.process_block(xB, yB, inside);
                */

                if (hsp < 0.0 && checkRight)
                {
                    if (NewPositionX > RightBlock - boundsx)
                    {
                        NewPositionX = RightBlock;
                        willreturn = false;

                        //map16_handler.process_block(xB, yB, right, pressed_y);
                        /* also, implement pipes please!
                        if (pad[button_left])
                        {
                            if (
                                lvl->gettile(xB, yB,1) == 319 &&
                                onGround
                                )
                            {
                                in_pipe = true;
                                pipe_speed_x = -2;
                                pipe_speed_y = 0;

                                ASM.Write_To_Ram(0x1DF9, 0x4, 1);
                            }
                        }
                        */
                    }
                }
                if (hsp > 0.0 && checkLeft)
                {
                    if (NewPositionX < LeftBlock + boundsx)
                    {
                        NewPositionX = LeftBlock;
                        willreturn = false;
                        //map16_handler.process_block(xB, yB, left, pressed_y);
                        /* yeah, more pipes
                        if (pad[button_right])
                        {
                            if (
                                map16_handler.get_tile(xB, yB) == 0x13F &&
                                ON_FL
                                )
                            {
                                in_pipe = true;
                                pipe_speed_x = 2;
                                pipe_speed_y = 0;
                                ASM.Write_To_Ram(0x1DF9, 0x4, 1);
                            }
                        }
                        */
                    }
                }
                if (vsp < 0.0 && checkTop)
                {
                    double bound_y = boundsy;
                    uint_fast8_t new_s = get_slope(xB, yB);
                    if (new_s != 0)
                    {
                        bound_y += 2;
                    }
                    if (NewPositionY > AboveBlock - bound_y)
                    {
                        willreturn = false;


                        if (new_s != 0 && slopetype == 0)
                        {
                            slopetype = new_s;
                        }

                        if (vsp <= 0)
                        {
                            NewPositionY = AboveBlock;
                        }

                        if ((spinjumping && lvl->gettile(xB, yB,1)->actas == 286) && playerstate > 0)
                        {
                            //map16_handler.process_block(xB, yB, top, pressed_y, true);
                            vsp = Calculate_Speed(768);
                            willreturn = true;
                        }
                        else
                        {
                            //map16_handler.process_block(xB, yB, top, pressed_y);
                        }
                        /* EVEN MORE PIPE SHINANIGANS
                        if (pad[button_down])
                        {
                            if (
                                lvl->gettile(uint_fast16_t((NewPositionX + 4.0) / 16.0), yB, 1)->actas == 0x137 &&
                                lvl->gettile(uint_fast16_t((NewPositionX + 12.0) / 16.0), yB, 1)->actas == 0x138
                                )
                            {
                                in_pipe = true;
                                pipe_speed_x = 0;
                                pipe_speed_y = -2;
                                ASM.Write_To_Ram(0x1DF9, 0x4, 1);
                            }
                        }
                        */
                    }
                }
                if (vsp > 0.0 && checkBottom)
                {
                    if (NewPositionY < BelowBlock + boundsy)
                    {
                        NewPositionY = BelowBlock;
                        willreturn = false;


                        if (playHitSound)
                        {
                            //TODO: sounds
                        }
                        //map16_handler.process_block(xB, yB, bottom);
                        if (pad[button_up])
                        {
                            //TODO: pipes
                        }
                    }
                }

            }
        }
    }

    x = NewPositionX;
    y = NewPositionY;
    return willreturn;
}
//now, this code is commented mainly because rendering isn't a focus right now.
//it's 11:35pm, i just want to get the barebones down to finish this tomorrow.
void mplayer::render(SDL_Renderer* render) {

    SDL_Rect rec = { camerax + x,cameray + y,16,16 };
    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
    SDL_RenderDrawRect(render, &rec);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
}
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

int mplayer::logic() {
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

    if (y < -lvl->height-32.0 && !dead) { //IF YOU FIND A BUG, CHANGE THE Y NUMBER HERE!
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
            if (!move(0.0, 1.0, true)) { //checks for floors
                if (vsp >= 0) {
                    onGround = true;
                    spinjumping = false;
                }
                else {
                    y -= 1;
                }
            }
            else { //no floor no pinecones
                y -= 1; //lol get dropped
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
                        vsp = -Calculate_Speed(1136.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                        //TODO: play sound here
                        spinjumping = true;
                    }
                    else
                    {
                        sliding = false;
                        //Normal jump
                        vsp = -Calculate_Speed(1232.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                        //TODO: play sound here

                    }
                }
            }
            if (pad[button_a] || pad[button_b]) {
                //this is actually a little known SMW feature i only learned about
                //a few years ago! holding A or B slows your fall
                //very interesting that like barely anyone knows about this
                //might need to tweak this number for kekcroc world 4
                gravity = gravity * 0.5;
            }
            double slopeadd = 0;
            if (!sliding) {
                if (slopetype == SLOPEHIGHR) {
                    if (!moving || (walk_dir == -1 && moving)) {
                        slopeadd = Calculate_Speed(-256);
                    }
                    if (moving) {
                        slopeadd = Calculate_Speed(-80);
                    }
                }
                if (slopetype == SLOPEHIGHL) {
                    if (!moving || (walk_dir == 1 && moving)) {
                        slopeadd = Calculate_Speed(256);
                    }
                    if (moving) {
                        slopeadd = Calculate_Speed(80);
                    }
                }

            }
            double xspeedtoset = slopeadd + (Calculate_Speed(320.0 + (running * 256.0) + (cansprint * 192.0)) * walk_dir) * moving;
            double speedaccx = Calculate_Speed(24.0);
            double stopspeed = Calculate_Speed(16.0);
            double skidspeed = Calculate_Speed(16.0 + (24.0 * running) + (cansprint * 40.0));

            if (sliding) { //FINALLY, slope sliding physics are handled here.
                xspeedtoset = 0;
                speedaccx = 0;
                stopspeed = Calculate_Speed(0x10);
                skidspeed = 0;
                if (slopetype == SLOPEHIGHR)
                {
                    xspeedtoset = Calculate_Speed(-0x300);
                    speedaccx = Calculate_Speed(0x18);
                }
                if (slopetype == SLOPEHIGHL)
                {
                    xspeedtoset = Calculate_Speed(0x300);
                    speedaccx = Calculate_Speed(0x18);
                }

                if (slopetype == 3 || slopetype == 4)
                {
                    xspeedtoset = Calculate_Speed(-0x2C0);
                    speedaccx = Calculate_Speed(0x10);
                }
                if (slopetype == 5 || slopetype == 6)
                {
                    xspeedtoset = Calculate_Speed(0x2C0);
                    speedaccx = Calculate_Speed(0x10);
                }
            }
            if (moving) { //and so begins our big physics time. Whenever
                //i begin to implement more characters like kitra, then
                //ill need to change this a LOT to fit their movement styles.
                //consider this temporary!
                if (hsp > 0.0 && xspeedtoset < 0.0 && walk_dir == -1) {
                    skidding = -1;
                    hsp -= skidspeed;
                }
                if (hsp < 0.0 && xspeedtoset > 0.0 && walk_dir == 1) {
                    skidding = 1;
                    hsp += skidspeed;
                }
                if (!onGround) {
                    skidding = 0;
                }
                if (hsp > xspeedtoset) {
                    hsp = max(xspeedtoset, hsp - speedaccx);

                }
                if (hsp < xspeedtoset) {
                    hsp = min(xspeedtoset, hsp + speedaccx);
                }

            }
            else {
                if (onGround == 1.0) {
                    if (!slopetype) {
                        if (hsp > 0.0) {
                            hsp = max(0, (hsp - stopspeed));
                        }
                        if (hsp < 0.0) {
                            hsp = min(0, hsp + stopspeed);
                        }

                    }
                    else {
                        if (hsp > xspeedtoset) {
                            hsp = max(xspeedtoset, (hsp - speedaccx));
                        }
                        if (hsp < xspeedtoset) {
                            hsp = min(xspeedtoset, hsp + speedaccx);
                        }
                    }
                }
            }
            if ((skidding || sliding)) {
                //TODO: implement particles here for skidding and sliding
            }
            if (!onGround || slopetype) {
                vsp -= Calculate_Speed(gravity);
            }
            else {
                prvious_hsp = hsp;
            }
            if (vsp > -Calculate_Speed(-1312.0)) {
                vsp = -Calculate_Speed(-1312.0);
            }
            if (!move(hsp + Calculate_Speed(double(vanillax) * 16.0), 0.0)) {
                hsp = 0.0;
            }
            if (!move(0.0, vsp - Calculate_Speed(double(vanillay) * 16.0))) {
                if (slopetype) {
                    if (vsp < 0) {
                        vsp = 0;
                    }
                    if (vsp > -Calculate_Speed(-768)) {
                        vsp = Calculate_Speed(-768);
                    }
                }
                else {
                    vsp = 0.0;
                }
            }

        } //aaaand this appears to conclude our physics related code for above water.
        //this was very interesting code to write, and i anticipate hundreds,if not thousands of bugs
        //fun stuff
        else { //jesus, this is the biggest 'else' of all time.

            sliding = false;
            spinjumping = false;
            onGround = false;
            if (!move(0.0, -1.0, true)) { //Detected a floor below

                if (vsp >= 0)
                {
                    onGround = true;
                    spinjumping = false;
                }
                else
                {
                    y -= 1;
                }
            }
            else {
                y -= 1;
            }

            shs = false;
            cansprint = false;
            moving = false;
            crouch = false;

            if (pad[button_left]) {
                walk_dir = -1;
                moving = true;
            }
            if (pad[button_right]) {
                walk_dir = 1;
                moving = true;
            }
            if (pad[button_down]) {
                if (onGround) { //TODO: this is yet another "we need grabbing" line...
                    walk_dir = 0;
                    moving = true;
                    crouch = true;
                }
            }

            if (((pad[button_a] || pad[button_b]) != lastjumped)) { //TODO: this is yet another "we need grabbing" line... {
                lastjumped = pad[button_a] || pad[button_b];
                if (lastjumped) {
                    //play sound effect here...
                    vsp -= Calculate_Speed(384);
                }
                spinjumping = false;
            }

            double SPEED_X_TO_SET = Calculate_Speed(256.0 / (1.0 + double(onGround))) * walk_dir;
            double SPEED_ACCEL_X = Calculate_Speed(24.0);
            double STOPPING_DECEL = Calculate_Speed(24.0);
            double SKID_ACCEL = Calculate_Speed(40.0);
            if (true) //TODO: implement grabbing HERE too!
            {
                vsp = vsp - Calculate_Speed(-8);
                vsp = max(Calculate_Speed(-1024), min(vsp, Calculate_Speed(384.0 - (pad[button_down] * 256.0) + (pad[button_up] * 384.0))));
            }
            else
            {
                vsp = min(1, vsp - Calculate_Speed(8));
                moving = true;
                double sp = 256 + (pad[button_right] || pad[button_left]) * 256;
                SPEED_ACCEL_X = Calculate_Speed(16.0);
                SPEED_X_TO_SET = Calculate_Speed(sp / (1.0 + double(onGround))) * walk_dir;

                if (pad[button_down])
                {
                    vsp = 1;
                    /*
                    if (!(global_frame_counter % 8)) //i um, still dont have a global frame counter,
                        //so im commenting this entire code block out for now.
                    {
                        RAM[0x1DF9] = 0x0E;
                    }
                    */
                }
            }


            /*
            Accel start
            */
            if (moving) {
                if (hsp > SPEED_X_TO_SET) {
                    hsp = max(SPEED_X_TO_SET, hsp - SPEED_ACCEL_X);
                }
                if (hsp < SPEED_X_TO_SET) {
                    hsp = min(SPEED_X_TO_SET, hsp + SPEED_ACCEL_X);
                }
                if (hsp > 0.0 && SPEED_X_TO_SET < 0.0 && walk_dir == -1) {
                    hsp -= SKID_ACCEL;
                }
                if (hsp < 0.0 && SPEED_X_TO_SET > 0.0 && walk_dir == 1) {
                    hsp += SKID_ACCEL;
                }
            }
            else {
                if (hsp > 0.0) {
                    hsp = max(0, hsp - STOPPING_DECEL);
                }
                if (hsp < 0.0) {
                    hsp = min(0, hsp + STOPPING_DECEL);
                }
            }
            /*
            Accel end
            */


            if (!move(hsp + Calculate_Speed(double(vanillax) * 16.0), 0.0)) {
                hsp = 0.0;
            }
            if (!move(0.0, hsp + Calculate_Speed(double(vanillay) * 16.0))) {
                vsp = 0;
            }
        }
        /*
            No more swimming
        */
    }

    ypressed = false;

    if (x < 8.0) {
        x = 8.0;
        hsp = 0;
    }
    if (x > double(-24 + lvl->width * 16)) {
        x = double(-24 + lvl->width * 16);

    }
    //TODO: grabbing items again
    return 1;

}
void mplayer::cameralogic() {
    if (dead || freezesprites) {
        return;
    }
    if (xcam == 0) {
        //this code basically means don't horizontally scroll the camera.
        //i haven't started on autoscroll so
        //TODO: implement autoscroll
    }
    else {
        //im gonna implement level endings myself, so i dont need ram for this
        double newx = x;
        if (smoothcam) {
            camerax += (newx - camerax) / smooth_camera_speed;
        }
        else {
            if (camerax < newx) {
                camerax += 5.0;
                if (camerax > newx) {
                    camerax = newx;
                }
            }

            if (camerax > newx) {
                camerax -= 5.0;
                if (camerax < newx) {
                    camerax = newx;
                }
            }

        }


    }
    if (ycam == 0) {
        //TODO: implement autoscroll
        //this has problems for the same reason as above.
    }
    else {
        double newy = (y + 16);

        if (smoothcam) {
            cameray += (newy - cameray) / smooth_camera_speed;
        }
        else {
            if (cameray < newy) {
                cameray += 5.0;
                if (cameray > newy) {
                    cameray = newy;
                }
            }

            if (cameray > newy) {
                cameray -= 5.0;
                if (cameray < newy) {
                    cameray = newy;
                }
            }

        }
    }
}






//THESE NUMBERS **WILL** CHANGE
//why am i using SMW tiles instead of enums? easy deployment, for the time being.
//enums ARE coming, and tiled support is the plan.
//why, might you ask, am i using a hyper realisitc smw engine to base this off of?
//guess i just hate myself lmao


double mplayer::getfloory(double truex, int blockx, int blocky) {
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
double mplayer::getfloorw(int blockx, int blocky) {

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
int mplayer::get_slope(int blockx, int blocky)
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

int mplayer::process()
{

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

    if (y < -32.0 && !dead) {
        die();
    }

    if (dead) {
        return deathlogic();
    }

    if (invinc_frames > 0) {
        invinc_frames -= 1;
    }
    hide = invinc_frames > 0 ? !hide : false;

    double GRAV = -double(GRAVITY);
    bool RUN = false;
    bool MOV = false;
    bool SLIGHT_HIGH_SPEED = false;



    uint_fast16_t check_x_1 = uint_fast16_t((x + 8) / 16.0);
    uint_fast16_t check_y_1 = uint_fast16_t((y + height) / 16.0);

    swim = lvl->gettile(check_x_1, check_y_1, 1)->actas < 4;

    if (swim != oldswim)
    {
        oldswim = swim;
        if (swim)
        {
            vsp = 0;
            hsp = 0;
            pmeter = 0;

            //Smoke
            //createParticle(0x60, 0x11, 0x8, 4, x, y - (STATE == 0) * 16.0, 0, 0, 0);
        }
    }



    if (false) {
        //in_pipe_process();
    }
    else
    {
        if (!swim)
        {
            if (onGround)
            {
                cansprint = false;
            }

            onGround = false;
            if (!move(0.0, -1.0, true)) { //Detected a floor below

                if (vsp <= 0)
                {
                    onGround = true;
                    spinjumping = false;
                }
                else
                {
                    y += 1;
                }
            }
            else {
                y += 1;
            }

            skidding = 0;
            if (abs(hsp) >= Calculate_Speed(576.0)) {

                SLIGHT_HIGH_SPEED = true;
                if (pad[button_y] && (pad[button_left] || pad[button_right]))
                {

                    if (cansprint || (onGround && !crouch)) {
                        pmeter += 2;
                        if (pmeter > P_METER_REQUIRED) {
                            pmeter = P_METER_REQUIRED;
                        }
                    }
                    else
                    {
                        if (pmeter > 0) {
                            pmeter -= 1;
                        }
                    }
                }
                else
                {
                    if (pmeter > 0) {
                        pmeter -= 1;
                    }
                }
            }
            else {
                if (pmeter > 0) {
                    pmeter -= 1;
                }
            }

            if (pmeter >= (P_METER_REQUIRED - 1) && pad[button_y]) {
                cansprint = true;
            }
            if (pad[button_left]) {
                walk_dir = -1;
                MOV = true;
            }
            if (pad[button_right]) {
                walk_dir = 1;
                MOV = true;
            }

            /*
                Slide cancel
            */
            if (sliding)
            {
                if ((MOV == true || (hsp == 0 && !slopetype)) && onGround)
                {
                    sliding = false;
                }
            }
            /*
                Crouching
            */
            if (pad[button_down] && !sliding) {
                if (slopetype != 0)
                {
                    if (true) //TODO: implement grabbing
                    {
                        if (MOV == false && !(pad[button_left] || pad[button_right]))
                        {
                            sliding = true;
                        }
                        else
                        {
                            crouch = false;
                        }
                    }
                    else
                    {
                        walk_dir = 0;
                        MOV = false;
                        crouch = true;
                    }
                }
                else
                {
                    if (onGround) {
                        walk_dir = 0;
                        MOV = true;
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
                RUN = true;
            }


            if ((pad[button_a] || pad[button_b]) != lastjumped) {
                lastjumped = pad[button_a] || pad[button_b];
                if (lastjumped && onGround) {
                    if (pad[button_a] && true) // IMPLEMENT GRABBING
                    {
                        sliding = false;
                        //Spinjump
                        vsp = Calculate_Speed(1136.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                        //play spinjump sound here
                        spinjumping = true;
                    }
                    else
                    {
                        sliding = false;
                        //Normal jump
                        vsp = Calculate_Speed(1232.0 + (abs(hsp) * 64.0)); //(148.0 * SLIGHT_HIGH_SPEED) + (32.0 * (X_SPEED > Calculate_Speed(320+256+176)))
                        //play jump sound here

                        //jump_is_spin = false;
                    }
                }
            }
            if (pad[button_a] || pad[button_b]) {
                GRAV = GRAV * 0.5;
            }

            double SLOPE_ADD = 0;
            if (!sliding)
            {
                if (slopetype == 1)
                {
                    if (!MOV || (walk_dir == -1 && MOV)) { SLOPE_ADD = Calculate_Speed(-256); }
                    if (MOV) { SLOPE_ADD = Calculate_Speed(-80); }
                }
                if (slopetype == 2)
                {
                    if (!MOV || (walk_dir == 1 && MOV)) { SLOPE_ADD = Calculate_Speed(256); }
                    if (MOV) { SLOPE_ADD = Calculate_Speed(80); }
                }
            }
            double SPEED_X_TO_SET = SLOPE_ADD + (Calculate_Speed(320.0 + (RUN * 256.0) + (cansprint * 192.0)) * walk_dir) * MOV;
            double SPEED_ACCEL_X = Calculate_Speed(24.0);
            double STOPPING_DECEL = Calculate_Speed(16.0);
            double SKID_ACCEL = Calculate_Speed(16.0 + (24.0 * RUN) + (cansprint * 40.0));

            /*
                Sliding down a slope :D
            */
            if (sliding)
            {
                SPEED_X_TO_SET = 0;
                SPEED_ACCEL_X = 0;
                STOPPING_DECEL = Calculate_Speed(0x10);
                SKID_ACCEL = 0;
                if (slopetype == 1)
                {
                    SPEED_X_TO_SET = Calculate_Speed(-0x300);
                    SPEED_ACCEL_X = Calculate_Speed(0x18);
                }
                if (slopetype == 2)
                {
                    SPEED_X_TO_SET = Calculate_Speed(0x300);
                    SPEED_ACCEL_X = Calculate_Speed(0x18);
                }

                if (slopetype == 3 || slopetype == 4)
                {
                    SPEED_X_TO_SET = Calculate_Speed(-0x2C0);
                    SPEED_ACCEL_X = Calculate_Speed(0x10);
                }
                if (slopetype == 5 || slopetype == 6)
                {
                    SPEED_X_TO_SET = Calculate_Speed(0x2C0);
                    SPEED_ACCEL_X = Calculate_Speed(0x10);
                }
            }

            /*
            Accel start
            */
            if (MOV) {
                if (hsp > 0.0 && SPEED_X_TO_SET < 0.0 && walk_dir == -1) {
                    skidding = -1;
                    hsp -= SKID_ACCEL;
                }
                if (hsp < 0.0 && SPEED_X_TO_SET > 0.0 && walk_dir == 1) {
                    skidding = 1;
                    hsp += SKID_ACCEL;
                }
                if (!onGround) {
                    skidding = 0;
                }
                if (hsp > SPEED_X_TO_SET) {
                    hsp = max(SPEED_X_TO_SET, hsp - SPEED_ACCEL_X);
                }

                if (hsp < SPEED_X_TO_SET) {
                    hsp = min(SPEED_X_TO_SET, hsp + SPEED_ACCEL_X);
                }
            }
            else {
                if (onGround == 1.0) {
                    if (!slopetype)
                    {
                        if (hsp > 0.0) {
                            hsp = max(0, hsp - STOPPING_DECEL);
                        }
                        if (hsp < 0.0) {
                            hsp = min(0, hsp + STOPPING_DECEL);
                        }
                    }
                    else
                    {

                        if (hsp > SPEED_X_TO_SET) {
                            hsp = max(SPEED_X_TO_SET, hsp - SPEED_ACCEL_X);
                        }

                        if (hsp < SPEED_X_TO_SET) {
                            hsp = min(SPEED_X_TO_SET, hsp + SPEED_ACCEL_X);
                        }
                    }
                }
            }
            /*
                Accel end
            */

            if ((skidding || sliding) /*&& !(global_frame_counter & 3)*/) //sorry, still no global framecounter...
            {
                //theres um, no particles either, sadly.
                // 
                //createParticle(0x3C, 0x00, 0x8, 3, x + 5.0, y - 27.0, 0, 0, 0, 0);
            }

            if (!onGround || slopetype) {
                vsp += Calculate_Speed(GRAV);
            }
            else {
                prvious_hsp = hsp;
            }
            if (vsp < Calculate_Speed(-1312.0)) {
                vsp = Calculate_Speed(-1312.0);
            }

            if (!move(hsp + Calculate_Speed(double(vanillax) * 16.0), 0.0)) {
                hsp = 0.0;
            }
            if (!move(0.0, vsp + Calculate_Speed(double(vanillay) * 16.0))) {
                if (slopetype)
                {
                    if (vsp > 0) { vsp = 0; }
                    if (vsp < Calculate_Speed(-768)) { vsp = Calculate_Speed(-768); }
                }
                else
                {
                    vsp = 0;
                }
            }
        }
        else
        {
            /*
            SWIMCODE
            */

            sliding = false;
            spinjumping = false;
            onGround = false;
            if (!move(0.0, -1.0, true)) { //Detected a floor below

                if (vsp <= 0)
                {
                    onGround = true;
                    spinjumping = false;
                }
                else
                {
                    y += 1;
                }
            }
            else {
                y += 1;
            }

            SLIGHT_HIGH_SPEED = false;
            cansprint = false;
            MOV = false;
            crouch = false;

            if (pad[button_left]) {
                walk_dir = -1;
                MOV = true;
            }
            if (pad[button_right]) {
                walk_dir = 1;
                MOV = true;
            }
            if (pad[button_down]) {
                if (onGround && true) { // still no grabbing, sorry guys!
                    walk_dir = 0;
                    MOV = true;
                    crouch = true;
                }
            }

            if (((pad[button_a] || pad[button_b]) != lastjumped) && true) { //yup, still no grabbing sadly
                lastjumped = pad[button_a] || pad[button_b];
                if (lastjumped) {
                    //RAM[0x1DF9] = 0x0E; play a sound effect here!
                    vsp += Calculate_Speed(384);
                }
                spinjumping = false;
            }

            double SPEED_X_TO_SET = Calculate_Speed(256.0 / (1.0 + double(onGround))) * walk_dir;
            double SPEED_ACCEL_X = Calculate_Speed(24.0);
            double STOPPING_DECEL = Calculate_Speed(24.0);
            double SKID_ACCEL = Calculate_Speed(40.0);
            if (true)
            {
                vsp = vsp + Calculate_Speed(-8);
                vsp = max(Calculate_Speed(-1024), min(vsp, Calculate_Speed(384.0 - (pad[button_down] * 256.0) + (pad[button_up] * 384.0))));
            }
            else
            {
                vsp = min(1, vsp + Calculate_Speed(8));
                MOV = true;
                double sp = 256 + (pad[button_right] || pad[button_left]) * 256;
                SPEED_ACCEL_X = Calculate_Speed(16.0);
                SPEED_X_TO_SET = Calculate_Speed(sp / (1.0 + double(onGround))) * walk_dir;

                if (pad[button_down])
                {
                    vsp = -1;
                    /*if (!(global_frame_counter % 8))
                    {
                        RAM[0x1DF9] = 0x0E;
                    }
                    this doesnt really work either*/
                }
            }


            /*
            Accel start
            */
            if (MOV) {
                if (hsp > SPEED_X_TO_SET) {
                    hsp = max(SPEED_X_TO_SET, hsp - SPEED_ACCEL_X);
                }
                if (hsp < SPEED_X_TO_SET) {
                    hsp = min(SPEED_X_TO_SET, hsp + SPEED_ACCEL_X);
                }
                if (hsp > 0.0 && SPEED_X_TO_SET < 0.0 && walk_dir == -1) {
                    hsp -= SKID_ACCEL;
                }
                if (hsp < 0.0 && SPEED_X_TO_SET > 0.0 && walk_dir == 1) {
                    hsp += SKID_ACCEL;
                }
            }
            else {
                if (hsp > 0.0) {
                    hsp = max(0, hsp - STOPPING_DECEL);
                }
                if (hsp < 0.0) {
                    hsp = min(0, hsp + STOPPING_DECEL);
                }
            }
            /*
            Accel end
            */


            if (!move(hsp + Calculate_Speed(double(vanillax) * 16.0), 0.0)) {
                hsp = 0.0;
            }
            if (!move(0.0, vsp + Calculate_Speed(double(vanillay) * 16.0))) {
                vsp = 0;
            }
        }
        /*
            No more swimming
        */
    }
    ypressed = false;
    //Get_Sprite();


    if (x < 8.0) { x = 8.0; hsp = 0; }
    if (x > double(-24 + lvl->width * 16)) { x = double(-24 + lvl->width * 16); }

    //ProcessGrabbed();
    return 1;
}

