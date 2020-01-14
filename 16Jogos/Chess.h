#pragma once

#include "Game.h"

class Chess : public Game
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

	Texture* figures[32];
	Texture* table;

	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2i oldPos = { 0, 0 };
	Vector2i newPos = { 0, 0 };
	std::string str;
	int n = 0;

	std::string position = "";

	float timer = 0, delay = 0.8f;

	Vector2i pos = {0, 0};

	int size = 56;

	int board[8][8] =
	{
		-1, -2, -3, -4, -5, -3, -2, -1,
		-6, -6, -6, -6, -6, -6, -6, -6,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 6,  6,  6,  6,  6,  6,  6,  6,
		 1,  2,  3,  4,  5,  3,  2,  1
	};

	bool collide(Texture* t, Vector2i pos);
	void loadPosition();
	std::string toChessNote(Vector2i p);
	Vector2i toCoord(char a, char b);
	void move(std::string str);
};

