#pragma once

#include "Game.h"

class Arkanoid : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	bool isCollide(Texture* t1, Texture* t2);

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
	Texture* ball;
	Texture* paddle;
	Texture* block[1000];

	int n = 0;
	int dx = 4, dy = -3;

	int bx = 300;
	int by = 300;

	int speed = 0;

	bool stopBall = true;

};


