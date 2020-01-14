#pragma once

#include "Game.h"

#include <list>

const int W = 1200;
const int H = 800;

const float DEGTORAD = 0.017453f;

class Animation
{
public:
	float Frame, speed;
	Texture* sprite;
	std::vector<SDL_Rect> frames;

	Animation() {}

	Animation(Texture* t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++)
		{
			SDL_Rect rect;
			rect.x = x + i * w;
			rect.y = y;
			rect.w = w;
			rect.h = h;
			frames.push_back(rect);
		}

		sprite = t;
		sprite->srcRect = frames[0];
	}

	void update()
	{
		Frame += speed;
		int n = int(frames.size());
		if (Frame >= n) Frame -= n;
		if (n > 0) sprite->srcRect = frames[int(Frame)];
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}
};

class Entity
{
public:
	int x, y, R;
	float dx, dy, angle;
	bool life;
	std::string name;
	Animation* anim;

	Entity() { life = 1;  }

	void settings(Animation* a, int X, int Y, float Angle = 0, int radius = 1)
	{
		x = X; y = Y; anim = a;
		angle = Angle; R = radius;
	}

	virtual void update() {}

	void draw(SDL_Renderer* renderer)
	{
		anim->sprite->destRect.x = x;
		anim->sprite->destRect.y = y;

		anim->sprite->draw(renderer, double(angle) + double(90));
	}
};

class asteroid : public Entity
{
public:
	asteroid()
	{
		dx = float(rand() % 8 - 4);
		dy = float(rand() % 8 - 4);
		name = "asteroid";
	}

	void update()
	{
		x += int(dx);
		y += int(dy);
		if (x > W) x = -50; if (x + 50 < 0) x = W;
		if (y > H) y = -50; if (y + 50 < 0) y = H;
	}
};

class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	void update()
	{
		dx = cos(angle * DEGTORAD) * 6;
		dy = sin(angle * DEGTORAD) * 6;
		x += int(dx);
		y += int(dy);
		if (x > W || x + 32 < 0 || y > H || y + 64 < 0) life = 0;
	}
};

class player : public Entity
{
public:
	bool thrust;

	player()
	{
		name = "player";
		thrust = false;
	}

	void update()
	{
		if (thrust)
		{
			dx += cos(angle * DEGTORAD) * 0.2f;
			dy += sin(angle * DEGTORAD) * 0.2f;
		}
		else
		{
			dx *= 0.99f;
			dy *= 0.99f;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx * dx + dy * dy);
		if (speed > maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += int(dx);
		y += int(dy);

		if (x > W) x = -40; if (x + 40 < 0) x = W;
		if (y > H) y = -40; if (y + 40 < 0) y = H;
	}

};

class Asteroids : public Game
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

	Texture* spaceship;
	Texture* bg;
	Texture* explosion;
	Texture* explosionShip;
	Texture* rock;
	Texture* fire;

	Animation* sRock;
	Animation* sBullets;
	Animation* sExplosion;
	Animation* sExplosion_ship;
	Animation* sPlayer;
	Animation* sPlayer_go;
	player* p;

	std::list<Entity*> entities;

	float x = float(W / 2 - 20), y = float(H / 2 - 20);
	float dx = 0, dy = 0, angle = 0;
	bool thrust;

	int pressedButtons[4] = { 0, 0, 0, 0 };

	float Frame = 0;
	float animSpeed = 0.4f;
	int frameCount = 20;

	bool isCollide(Entity* a, Entity* b);
};