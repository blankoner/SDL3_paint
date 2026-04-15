#include "include/interface.h"

void Interface::clearWindow(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void Interface::drawSquare(SDL_Renderer* renderer, float x, float y, float size, SDL_Color color)
{
	SDL_FRect square{x, y, size, size};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &square);
}
