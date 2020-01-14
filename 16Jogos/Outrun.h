#pragma once

#include "Game.h"

class Outrun : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	void drawQuad(int* color, int x1, int y1, int w1, int x2, int y2, int w2);

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

	int pressedButtons[4] = {0, 0, 0, 0};

};