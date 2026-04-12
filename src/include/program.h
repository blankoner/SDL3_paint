#pragma once

#include "interface.h"
#include "coordinates.h"

#include <vector>

class Program
{
public:
	Program(int width, int height);
	~Program();
	void run();

private:
	int width;
	int height;
	int brushSize;
	bool quit;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Coordinates coords;
	// coords, size of brush in that moment
	std::vector<std::pair<Coordinates, int>> trace;

	void renderTrace();
	void clearTrace();

	// Conditions
	bool mouseInsideWindow(const float& x, const float& y);	

	// Inputs
	void handleScancodes();
	void handleEvents();
	void handleInput();
	void handleMouse();
};
