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
	
	window = SDL_CreateWindow("Paint -D", width, height, 0);
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
	brush.color = {0, 0, 0, 255};
}

void Program::setBrushColor(colorPalette color)
{
	switch(color)
	{
		case colorPalette::Red:
			brush.color = {255, 0, 0};
			break;
		case colorPalette::Green:
			brush.color = {0, 255, 0};
			break;
		case colorPalette::Blue:
			brush.color = {0, 0, 255};
			break;
		case colorPalette::White:
			brush.color = {255, 255, 255};
			break;
		case colorPalette::Black:
			brush.color = {0,0,0};
			break;
	}
}

void Program::renderPalette()
{
	const int paintSize = 20;
	const int margin = 3;
	const SDL_Color colors[paintsNum] = {
		{255, 0, 0, 255},
		{0, 255, 0, 255},
		{0, 0, 255, 255},
		{255, 255, 255, 255},
		{0, 0, 0, 255}
	};

	for(int i = 0; i < paintsNum; i++)
	{
		paints[i] = {(margin + i * (paintSize + margin)), margin, paintSize, colors[i]};
		Interface::drawSquare(renderer, paints[i].x, paints[i].y, paints[i].size, paints[i].color);
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

bool Program::mouseOverPalette()
{
	float x = 0;
	float y = 0;
	SDL_GetMouseState(&x, &y);
	
	for(int i = 0; i < paintsNum; i++)
	{
		if(x >= paints[i].x && x <= paints[i].x + paints[i].size
				&& y >= paints[i].y && y <= paints[i].y + paints[i].size)
			return true;
	}

	return false;
}

void Program::colorPick()
{
	float x = 0;
	float y = 0;
	SDL_GetMouseState(&x, &y);
	std::vector<colorPalette> colors = {colorPalette::Red,
																			colorPalette::Green,
																			colorPalette::Blue,
																			colorPalette::White,
																			colorPalette::Black};
	for(int i = 0; i < paintsNum; i++)
	{
		if(x >= paints[i].x && x <= paints[i].x + paints[i].size 
				&& y >= paints[i].y && y <= paints[i].y + paints[i].size)
		setBrushColor(colors[i]);
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
	
	bool overPalette = mouseOverPalette();
	
	if(mouseInsideWindow)
	{
		if(!overPalette)
		{
			Interface::drawSquare(renderer, x, y, brush.size, brush.color);
			if(mouseButtonHold)
			{
				brush.x = x;
				brush.y = y;
				trace.push_back(brush);
			}
		}
		else
		{
			if(mouseButtonHold)
			{
				colorPick();
			}
		}
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
		renderPalette();

		handleInput();
		
		renderTrace();

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
}
