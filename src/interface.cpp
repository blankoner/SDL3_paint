#include "include/interface.h"

void Interface::clearWindow(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	drawPalettePane(renderer);
}

void Interface::drawSquare(SDL_Renderer* renderer, float x, float y, float a)
{
	SDL_FRect square{x, y, a, a};
	SDL_RenderFillRect(renderer, &square);
}

void Interface::drawPalettePane(SDL_Renderer* renderer)
{
	const int paintSize = 20;
	const int paintsNum = 5;
	const int margin = 3;
	const SDL_Color colors[5] = {
		{255, 0, 0, 255}, 
		{0, 255, 0, 255}, 
		{0, 0, 255, 255}, 
		{255, 255, 255, 255}, 
		{0, 0, 0, 255}
	};

	for(int i = 0; i < paintsNum; i++)
	{
		SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
		drawSquare(renderer, (i * (paintSize + margin)) + margin, margin, paintSize);
	}
}
