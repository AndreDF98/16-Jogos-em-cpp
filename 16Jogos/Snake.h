#pragma once

#include "Game.h"

class SnakeGame : public Game
{

public:

	void init() override;

	void handleEvents() override;
	void update() override;
	void render() override;
	void clean() override;

	void Tick();

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

	Texture* white;
	Texture* red;
	Texture* green;

	int N = 30, M = 20;
	int size = 16;

	int dir, num = 4;

	struct Snake
	{
		int x, y;
	} s[100];

	struct Fruit
	{
		int x = 10, y = 10;
	} f;

	float timer = 0.0f, delay = 0.1f, speed = 0.02f;

};