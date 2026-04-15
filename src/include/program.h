#pragma once

#include "interface.h"
#include "brush.h"
#include "colorPalette.h"
#include "paint.h"

#include <array>
#include <vector>

class Program
{
public:
	Program(int width, int height);
	~Program();
	void run();

private:
	// Window
	int width;
	int height;
	bool quit;
	SDL_Window* window;
	SDL_Renderer* renderer;
	static const int paintsNum = 5;
	std::array<Paint, paintsNum> paints;

	void renderPalette();

	// Brush
	Brush brush;
	std::vector<Brush> trace;

	void renderTrace();
	void clearTrace();
	void setDefaultBrush();
	void setBrushColor(colorPalette color);
	void colorPick();

	// Conditions
	bool mouseButtonHold;
	bool mouseInsideWindow;
	bool mouseOverPalette();

	// Inputs
	void handleScancodes();
	void handleEvents();
	void handleMouse();
	void handleInput();
};
