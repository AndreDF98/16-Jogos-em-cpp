#include "DoodleJump.h"

SDL_Renderer* DoodleJump::renderer = nullptr;

SDL_Event DoodleJump::event;

void DoodleJump::init(const char* title, int width, int height, bool fullscreen)
{
	srand((unsigned int)time(0));

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
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
	background->load(renderer, "assets/doodle/background.png");
	platform = new Texture();
	platform->load(renderer, "assets/doodle/platform.png");
	pers = new Texture();
	pers->load(renderer, "assets/doodle/doodle.png");

	//config bg texture size
	background->srcRect.x =		background->srcRect.y =		0;
	background->destRect.x =	background->destRect.y =	0;
	background->srcRect.w =		background->destRect.w =	400;
	background->srcRect.h =		background->destRect.h =	533;

	//config platform texture size
	platform->srcRect.x =	platform->srcRect.y =	0;
	platform->srcRect.w =	platform->destRect.w =	68;
	platform->srcRect.h =	platform->destRect.h =	14;

	//config pers texture size
	pers->srcRect.x =	pers->srcRect.y =	0;
	pers->srcRect.w =	pers->destRect.w =	80;
	pers->srcRect.h =	pers->destRect.h =	80;

	for (int i = 0; i < 10; i++)
	{
		plat[i].x = float(rand() % 400);
		plat[i].y = float(rand() % 533);
	}
}

void DoodleJump::handleEvents()
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
				dx = -1;
				flip = false;
				break;
			case SDLK_d:
				dx = 1;
				flip = true;
				break;
			case SDLK_s:
				break;
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
				dx = 0;
				break;
			case SDLK_d:
				dx = 0;
				break;
			case SDLK_s:
				break;
			default:
				break;
			}
		}
	}
}

void DoodleJump::update()
{
	dy += 0.2f;
	y += dy;

	x += dx * 5;

	if (y > 500) dy = -10;

	if (y < h)
		for (int i = 0; i < 10; i++)
		{
			y = h;
			plat[i].y = plat[i].y - dy;
			if (plat[i].y > 533)
			{
				plat[i].y = 0;
				plat[i].x = float(rand() % 400);
			}
		}

	for (int i = 0; i < 10; i++)
		if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68) && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0)) dy = -10;

	pers->destRect.x = int(x);
	pers->destRect.y = int(y);
}

void DoodleJump::render()
{
	SDL_RenderClear(renderer);

	background->draw(renderer);
	if (!flip) pers->draw(renderer);
	else pers->flipH(renderer);

	for (int i = 0; i < 10; i++)
	{
		platform->destRect.x = int(plat[i].x);
		platform->destRect.y = int(plat[i].y);
		platform->draw(renderer);
	}

	SDL_RenderPresent(renderer);
}

void DoodleJump::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}