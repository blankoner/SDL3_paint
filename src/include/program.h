#pragma once

#include "interface.h"
#include "brush.h"
#include "colorPalette.h"

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
	Brush brush;
	bool quit;
	SDL_Window* window;
	SDL_Renderer* renderer;
	// Brush parameters at particular moment
	std::vector<Brush> trace;

	// Brush
	void renderTrace();
	void clearTrace();
	void setDefaultBrush();
	void setBrushColor(colorPalette color);

	// Conditions
	bool mouseButtonHold;
	bool mouseInsideWindow;

	// Inputs
	void handleScancodes();
	void handleEvents();
	void handleInput();
	void handleMouse();
};
