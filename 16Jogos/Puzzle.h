#pragma once

#include "Game.h"

class PuzzleGame : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

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

	Texture* tiles[20];

	int w = 64;
	int grid[6][6] = { 0 };
};