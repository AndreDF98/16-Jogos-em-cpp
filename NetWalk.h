#pragma once

#include "Game.h"

#include <vector>

class NetWalk : public Game
{

public:

	void init(const char* title, int width, int height, bool fullscreen) override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	bool running() override
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

	Texture* pipes;
	Texture* background;
	Texture* computer;

	int ts = 54;
	int offset[2] = { 65, 55 };

	bool click = false;
};

