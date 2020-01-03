#pragma once

#include "Game.h"

class TetrisGame : public Game
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

	Texture* tileTex;
	Texture* bgTex;
	Texture* frameTex;

	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;

	float delay = 0.3f;
	float speed = 0.02f;
	float timer;

};

