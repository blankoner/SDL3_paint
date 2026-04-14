#include "include/program.h"
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

bool Program::mouseInsideWindow(const float& x, const float& y)
{
	if(x >= 0 && x <= (width - brush.size) && y >= 0 && y <= (height - brush.size))
		return true;

	return false;
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
			}
		}
}

void Program::renderTrace()
{
	if(!trace.empty())
	{
		for(const auto &step : trace)
		{
			Interface::drawSquare(renderer, step.x, step.y, step.size);
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

	if(mouseInsideWindow(x, y))
	{
		Interface::drawSquare(renderer, x, y, brush.size);
	}
	if(mouseInsideWindow(x, y) && mouseButtonHold)
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
