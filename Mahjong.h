#pragma once

#include "Game.h"

class Mahjong : public Game
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

	Texture* tiles;
	Texture* background;

	int w = 48, h = 66;
	int stepX = w / 2 - 2, stepY = h / 2 - 2;
	float offX = 4.6f, offY = 7.1f; //z offset

	bool click = false;
};

