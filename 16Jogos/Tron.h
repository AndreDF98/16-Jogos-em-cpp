#pragma once

#include "Game.h"

#include <fontsub.h>

class Tron : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	void drawQuad(SDL_Color color, int x, int y, int w);

	bool running() override
	{
		return isRunning;
	}

	static SDL_Renderer* renderer;
	static SDL_Event event;

private:
	bool isRunning;
	bool isPaused = false;
	SDL_Window* window;
	Mix_Music* music;

	Texture* bg;

	int pressedButtonsP1[4] = { 0, 0, 0, 0 };
	int pressedButtonsP2[4] = { 0, 0, 0, 0 };
	
};

