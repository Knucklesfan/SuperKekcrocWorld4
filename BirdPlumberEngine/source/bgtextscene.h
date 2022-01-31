#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "message.h"
#include "scene.h"
#include "background.h"
#include "font.h"

class bgtextscene : public scene {

	public:

        std::vector<std::string> words = {
            "@2Kekcroc:\n@0Hello World! I'm kekcroc, nice to meet you.",
            "@1Mario:\n@0Can you believe it? @4Christmas, just a week away",
            "@2Kekcroc:\n@0This is a very long text. How long? Extremely. Intentionally long, too. If I can't write long text, then what's even the point of being a programmer? Dude, imagine if text was long. There's no way it could be long, Kekcroc. But yeah, but what if it was long. DUUDE! long text is like, a foresaken art, only learned by the greatest of programmers. Yeah dude, this code is crap, there's no way that it can be considered being written by \"the greatest programmer of all time\" or anything lol",
            "@1Mario:\n@2Kekcroc, @0did you @3really @0have to @5break @0the text box @7like that?",
            "@2Kekcroc:\n@0Yes.",
        };
        std::vector<std::vector<int>> params = {
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {2,0,0},
        };
        int nextbg = 0;
        int currentbg = 0;
        double ticks = 0;
        double transitionalpha = 1.0;
        bool up = false;
        bool down = false;
        bool transitioning = false;
        bool active = false;
		bgtextscene(SDL_Renderer* render, std::vector<SDL_Texture*> texture, std::vector<bg*>  backg, std::vector<Mix_Chunk*> soundVec, std::vector<font*>  font);
		std::vector<bg*> backgrounds;
		std::vector<Mix_Chunk*> sounds;
		std::vector<font*> fonts;
		message* msg;
		void render(SDL_Renderer* renderer);
		void logic(double deltatime);
		void keyPressed(SDL_Keycode);
        void movebackgrounds(int tomove);
		int endlogic();
		void reset();
};

