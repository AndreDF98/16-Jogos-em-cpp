#pragma once

#include "Game.h"

struct point
{
	float x, y;
};

class DoodleJump : public Game
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

	Texture* background;
	Texture* platform;
	Texture* pers;

	point plat[20];

	float x = 100.0f, y = 100.0f, h = 200.0f;
	float dx = 0.0f, dy = 0.0f;

	bool flip = false;

};

