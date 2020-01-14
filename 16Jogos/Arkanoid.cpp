#include "Arkanoid.h"

SDL_Renderer* Arkanoid::renderer = nullptr;

SDL_Event Arkanoid::event;

bool Arkanoid::isCollide(Texture* t1, Texture* t2)
{
	SDL_Rect a = t1->destRect;
	SDL_Rect b = t2->destRect;
	SDL_Rect result;

	if (SDL_IntersectRect(&a, &b, &result) == SDL_TRUE) return true;
	else return false;
}

void Arkanoid::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 520, 450, 0);
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
		isPaused = false;
	}
	else
	{
		isRunning = false;
	}

	//load textures
	background = new Texture();
	background->load(renderer, "assets/arkanoid/background.png");
	ball = new Texture();
	ball->load(renderer, "assets/arkanoid/ball.png");
	paddle = new Texture();
	paddle->load(renderer, "assets/arkanoid/paddle.png");

	//config bg texture size
	background->srcRect.x = background->srcRect.y = 0;
	background->destRect.x = background->destRect.y = 0;
	background->srcRect.w = 520;
	background->destRect.w = 520;
	background->srcRect.h = background->destRect.h = 450;

	//config paddle texture size
	paddle->srcRect.x = paddle->srcRect.y = 0;
	paddle->destRect.x = 300;
	paddle->destRect.y = 440;
	paddle->srcRect.w = paddle->destRect.w = 90;
	paddle->srcRect.h = paddle->destRect.h = 9;

	//config paddle texture size
	ball->srcRect.x = ball->srcRect.y = 0;
	ball->srcRect.w = ball->destRect.w = 12;
	ball->srcRect.h = ball->destRect.h = 12;

	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			//load block texture
			block[n] = new Texture();
			block[n]->load(renderer, "assets/arkanoid/block01.png");
			//config block texture size
			block[n]->srcRect.x = block[n]->srcRect.y = 0;
			block[n]->destRect.x = i * 43;
			block[n]->destRect.y = j * 20;
			block[n]->srcRect.w = block[n]->destRect.w = 42;
			block[n]->srcRect.h = block[n]->destRect.h = 20;
			n++;
		}

}

void Arkanoid::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}

	if (isPaused == false)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				break;
			case SDLK_a:
				speed = -1;
				break;
			case SDLK_d:
				speed = 1;
				break;
			case SDLK_s:
				break;
			case SDLK_SPACE:
				stopBall = false;
			default:
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				break;
			case SDLK_a:
				speed = 0;
				break;
			case SDLK_d:
				speed = 0;
				break;
			case SDLK_s:
				break;
			default:
				break;
			}
		}
	}
}

void Arkanoid::update()
{
	paddle->destRect.x += speed * 5;
	if (paddle->destRect.x < 0) paddle->destRect.x = 0;
	if (paddle->destRect.x+ paddle->destRect.w > 520) paddle->destRect.x = 520 - paddle->destRect.w;

	ball->destRect.x = bx;
	ball->destRect.y = by;

	if (stopBall == false)
	{
		bx += dx;
		by += dy;


		for (int i = 0; i < n; i++)
			if (isCollide(ball, block[i]))
			{
				dy = -dy;
				block[i]->destRect.x = -100;
				block[i]->destRect.y = 0;
			}

		if (isCollide(paddle, ball)) dy = -(rand() % 5 + 2);

		if (bx < 0 || bx > 520) dx = -dx;
		if (by < 0) dy = -dy;
		if (by > 450)
		{
			stopBall = true;
			by = paddle->destRect.y - ball->destRect.h;
			bx = paddle->destRect.x + paddle->destRect.w / 2;
			dy = -dy;
		}
	}
	else
	{
		by = paddle->destRect.y - ball->destRect.h;
		bx = paddle->destRect.x + paddle->destRect.w / 2;
	}

}

void Arkanoid::render()
{
	SDL_RenderClear(renderer);

	background->draw(renderer);
	ball->draw(renderer);
	paddle->draw(renderer);

	for (int i = 0; i < n; i++)
		block[i]->draw(renderer);

	SDL_RenderPresent(renderer);
}

void Arkanoid::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}