#pragma once

#include "Game.h"

class chooseGame : public Game
{

public:

	void init(const char* title, int width, int height, bool fullscreen) override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	bool isIn(Texture* t, Vector2i pos);

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

};

