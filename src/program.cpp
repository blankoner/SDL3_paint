#include "include/program.h"
#include "include/colorPalette.h"
#include <stdexcept>

Program::Program(int width, int height)
{
	this->width = width;
	this->height = height;

	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		throw::std::runtime_error(SDL_GetError());
	}
	
	window = SDL_CreateWindow("paint", width, height, 0);
	renderer = SDL_CreateRenderer(window, nullptr);
	
	setDefaultBrush();
	quit = false;
}

Program::~Program()
{
	if(window)
		SDL_DestroyWindow(window);
	if(renderer)
		SDL_DestroyRenderer(renderer);

	SDL_Quit();
}

void Program::setDefaultBrush()
{
	brush.size = 20;
	brush.color.r = 0;
	brush.color.g = 0;
	brush.color.b = 0;
	brush.color.a = 255;
}

void Program::setBrushColor(colorPalette color)
{
	switch(color)
	{
		case colorPalette::Red:
			brush.color.r = 255;
			brush.color.g = 0;
			brush.color.b = 0;
			break;
		case colorPalette::Green:
			brush.color.r = 0;
			brush.color.g = 255;
			brush.color.b = 0;
			break;
		case colorPalette::Blue:
			brush.color.r = 0;
			brush.color.g = 0;
			brush.color.b = 255;
			break;
		case colorPalette::White:
			brush.color.r = 255;
			brush.color.g = 255;
			brush.color.b = 255;
			break;
		case colorPalette::Black:
			setDefaultBrush();
			break;
	}
}

void Program::renderTrace()
{
	if(!trace.empty())
	{
		for(const auto &step : trace)
		{
			Interface::drawSquare(renderer, step.x, step.y, step.size, step.color);
		}
	}
}

void Program::clearTrace()
{
	if(!trace.empty())
	{
		trace.clear();
	}
}

void Program::handleEvents()
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_EVENT_KEY_DOWN:
				switch(e.key.key)
				{
					case SDLK_Q:
						quit = true;
						break;
					case SDLK_R:
						setBrushColor(colorPalette::Red);
						break;
					case SDLK_G:
						setBrushColor(colorPalette::Green);
						break;
					case SDLK_B:
						setBrushColor(colorPalette::Blue);
						break;
					case SDLK_W:
						setBrushColor(colorPalette::White);
						break;
					case SDLK_K:
						setBrushColor(colorPalette::Black);
						break;
				}
				break;
			case SDL_EVENT_KEY_UP:
				switch(e.key.key)
				{
					case SDLK_SPACE:
						clearTrace();
						break;
				}
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mouseButtonHold = true;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				mouseButtonHold = false;
				break;
			case SDL_EVENT_WINDOW_MOUSE_ENTER:
				mouseInsideWindow = true;
				break;
			case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				mouseInsideWindow = false;
				break;
		}
	}
}


void Program::handleScancodes()
{
	const int brushScaler = 1;
	const int maxBrushSize = 50;
	const int minBrushSize = 1;
	const bool* keyStates = SDL_GetKeyboardState(nullptr);

	if(keyStates[SDL_SCANCODE_UP])
	{
		if(brush.size < maxBrushSize)
			brush.size += brushScaler;
	}

	if(keyStates[SDL_SCANCODE_DOWN])
	{
		if(brush.size > minBrushSize)
			brush.size -= brushScaler;
	}
}

void Program::handleMouse()
{
	float x = 0;
	float y = 0;

	SDL_GetMouseState(&x, &y);

	if(mouseInsideWindow)
	{
		Interface::drawSquare(renderer, x, y, brush.size, brush.color);
	}
	if(mouseInsideWindow && mouseButtonHold)
	{
		brush.x = x;
		brush.y = y;
		trace.push_back(brush);
	}
}

void Program::handleInput()
{
	handleMouse();
	handleScancodes();
	handleEvents();
}

void Program::run()
{
	while(!quit)
	{
		Interface::clearWindow(renderer);
		handleInput();

		renderTrace();

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
}
