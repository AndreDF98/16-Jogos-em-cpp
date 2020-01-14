#pragma once

#include "Game.h"

#include <string>

class chooseGame : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	int gameChoose = 0;

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

	Texture* bg;
	Texture* button[16];

	Vector2i pos = {0, 0};

	bool isIn(Texture* t, Vector2i pos);

};

