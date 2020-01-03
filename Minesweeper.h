#pragma once

#include "Game.h"

class Minesweeper : public Game
{

public:

	void init(const char* title, int width, int height, bool fullscreen) override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	void clickTile(int i, int j);

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

	int w = 32;
	int grid[12][12];
	int sgrid[12][12]; //for showing

	int x, y;

	Texture* tiles;

};