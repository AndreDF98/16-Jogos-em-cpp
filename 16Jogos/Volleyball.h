#pragma once

#include "Game.h"

#include "box2d/box2d.h"

class Volleyball : public Game
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

	const float SCALE = 30.f;
	const float DEG = 57.29577f;

	Texture* bg;
	Texture* ball;
	Texture* blobby;

	void setWall(int x, int y, int w, int h);

	b2Body* b;
	b2Body* pBody[2];
	b2Vec2 pos;
	b2Vec2 vel;

	int pressedButtonsP1[4] = { 0, 0, 0, 0 };
	int pressedButtonsP2[4] = { 0, 0, 0, 0 };

};

