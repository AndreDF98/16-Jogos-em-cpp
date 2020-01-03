#pragma once

#include "Game.h"

class Bejeweled : public Game
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

	int ts = 54; //tile size
	int offset[2] = { 48, 24 };

	Texture* background;
	Texture* gems;

	int x0, y0, x, y;
	int click = 0;
	int pos[2];

	bool isSwap = false, isMoving = false;

};