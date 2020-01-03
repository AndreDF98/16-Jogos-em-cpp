#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Texture.h"

#include <time.h>
#include <iostream>

class Game
{

public:

	virtual void init(const char* title, int width, int height, bool fullscreen) {}

	virtual void handleEvents() {}
	virtual void update() {}
	virtual void render() {}
	virtual void clean() {}

	virtual bool running()
	{
		return isRunning;
	}

	static SDL_Renderer* renderer;
	static SDL_Event event;

private:
	bool isRunning;
	bool isPaused;
	SDL_Window* window;
	Mix_Music* music;

};

