#pragma once

#include <SDL3/SDL.h>

class Interface
{
public:
	Interface(){}
	~Interface(){}
	static void drawSquare(SDL_Renderer* renderer, float x, float y, float a);
	static void clearWindow(SDL_Renderer* renderer);

private:
	static void drawPalettePane(SDL_Renderer* renderer);
};
