#include <SDL2/SDL.h>

namespace util {
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center, SDL_Renderer* render);
};