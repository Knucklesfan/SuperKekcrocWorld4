#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>


namespace util {
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center,  SDL_RendererFlip flip, SDL_Renderer* render); 
	//old crappy draw texture which is boring and dumb
	//this one IS faster though.
	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_RendererFlip flip, int srcx, int srcy, int srcw, int srch); 
	// new draw txture with spritesheets
	// slower, but better!
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_RendererFlip flip, int srcx, int srcy, int srcw, int srch, int scalex, int scaley); 
	// new draw txture with spritesheets
	// slower, but better!

	SDL_Texture* generateTexture(std::string path, SDL_Renderer* renderer);

	std::vector<std::string> seperateWords(std::string string, char sep);
	std::vector<std::string> seperateWords(std::string string, char sep, int);
	std::string wrap(std::string str, int pixels);
	int nthOccurrence(const std::string& str, const std::string& findMe, int nth);
	
	//gets a tile at coordinates in particular.
	
};