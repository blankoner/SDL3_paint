#include "include/program.h"
#include "include/interface.h"
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
	brushSize = 20;
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

bool Program::mouseInsideWindow(const float& x, const float& y)
{
	if(x >= 0 && x <= (width - brushSize) && y >= 0 && y <= (height - brushSize))
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
					coords.x = e.button.x;
					coords.y = e.button.y;
					trace.push_back(std::make_pair(coords, brushSize));
					break;
			}
		}
}

void Program::renderTrace()
{
	if(!trace.empty())
	{
		for(auto step : trace)
		{
			Interface::drawSquare(renderer, step.first.x, step.first.y, step.second);
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
	const int brushStep = 1;
	const int maxBrushSize = 50;
	const int minBrushSize = 1;
	const bool* keyStates = SDL_GetKeyboardState(nullptr);

	if(keyStates[SDL_SCANCODE_UP])
	{
		if(brushSize < maxBrushSize)
			brushSize += brushStep;
	}

	if(keyStates[SDL_SCANCODE_DOWN])
	{
		if(brushSize > minBrushSize)
			brushSize -= brushStep;
	}
}

void Program::handleMouse()
{
	float x = 0;
	float y = 0;

	SDL_GetMouseState(&x, &y);

	if(mouseInsideWindow(x, y))
	{
		Interface::drawSquare(renderer, x, y, brushSize);
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
