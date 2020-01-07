#pragma once

#include "SDL.h"
#include "SDL_mixer.h"
#include "Texture.h"
#include "SDL_ttf.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>

class Game
{

public:

	virtual void init(const char* title, int width, int height, bool fullscreen) {}

	virtual void handleEvents() {}
	virtual void update() {}
	virtual void render() {}
	virtual void clean() {}

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

};

struct Vector2i
{
	int x, y;

	Vector2i(int a, int b)
	{
		x = a;
		y = b;
	}

	Vector2i& operator=(const Vector2i& a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}

	Vector2i operator+(const Vector2i& a) const
	{
		return Vector2i(a.x + x, a.y + y);
	}

	Vector2i operator-(const Vector2i& a) const
	{
		return Vector2i(a.x - x, a.y - y);
	}

	Vector2i operator*(const Vector2i& a) const
	{
		return Vector2i(a.x * x, a.y * y);
	}

	Vector2i operator*(int a) const
	{
		return Vector2i(a * x, a * y);
	}

	bool operator==(const Vector2i& a) const
	{
		return (x == a.x && y == a.y);
	}
};

struct Vector3i
{
	int x, y, z;

	Vector3i()
	{
		x = NULL;
		y = NULL;
		z = NULL;
	}

	Vector3i(int a, int b, int c)
	{
		x = a;
		y = b;
		z = c;
	}

	Vector3i& operator=(const Vector3i& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	Vector3i operator+(const Vector3i& a) const
	{
		return Vector3i(a.x + x, a.y + y, a.z + z);
	}

	Vector3i operator-(const Vector3i& a) const
	{
		return Vector3i(a.x - x, a.y - y, a.z - z);
	}

	Vector3i operator*(const Vector3i& a) const
	{
		return Vector3i(a.x * x, a.y * y, a.z * z);
	}

	Vector3i operator*(int a) const
	{
		return Vector3i(a * x, a * y, a * z);
	}

	bool operator==(const Vector3i& a) const
	{
		return (x == a.x && y == a.y && z == a.z);
	}

};