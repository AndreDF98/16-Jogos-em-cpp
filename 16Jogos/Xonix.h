#pragma once

#include "Game.h"

class Xonix : public Game
{

public:

	void init() override;

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
	Texture* gameOver;
	Texture* enemy;

	int pressedButtons[4] = { 0, 0, 0, 0 };

	int x = 0, y = 0, dx = 0, dy = 0;
	float timer = 0, delay = 0.1f, speed = 0.02f;

	int enemyCount = 4;
	int enemyAngle = 0;
};