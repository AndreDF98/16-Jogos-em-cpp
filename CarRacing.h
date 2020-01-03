#pragma once

#include "Game.h"

class CarRacing : public Game
{

public:

	void init(const char* title, int width, int height, bool fullscreen) override;

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

	Texture* car;
	Texture* background;

	float x = 300, y = 300;
	float speed = 0, angle = 0;
	float maxSpeed = 12.0f;
	float acc = 0.2f; 
	float dec = 0.3f;
	float turnSpeed = 0.08f;

	int offsetX = 0, offsetY = 0;

	bool Up = 0, Right = 0, Down = 0, Left = 0;

};