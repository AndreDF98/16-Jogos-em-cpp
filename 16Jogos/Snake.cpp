#include "Snake.h"

SDL_Renderer* SnakeGame::renderer = nullptr;

SDL_Event SnakeGame::event;

void SnakeGame::Tick()
{
	for (int i = num; i > 0; --i)
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	if (dir == 0) s[0].y += 1;
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y -= 1;

	if ((s[0].x == f.x) && (s[0].y == f.y))
	{
		num++;
		f.x = rand() % N;
		f.y = rand() % M;
	}

	if (s[0].x >= N) s[0].x = 0;
	if (s[0].x < 0) s[0].x = N;
	if (s[0].y >= M) s[0].y = 0;
	if (s[0].y < 0) s[0].y = M;

	for (int i = 1; i < num; i++)
		if (s[0].x == s[i].x && s[0].y == s[i].y) num = i;
}

void SnakeGame::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 320, 0);
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
	white = new Texture();
	white->load(renderer, "assets/snake/white.png");
	red = new Texture();
	red->load(renderer, "assets/snake/red.png");
	green = new Texture();
	green->load(renderer, "assets/snake/green.png");

	//config white texture size
	white->srcRect.x = white->srcRect.y = 0;
	white->srcRect.w = white->destRect.w = 16;
	white->srcRect.h = white->destRect.h = 16;

	//config red texture size
	red->srcRect.x = red->srcRect.y = 0;
	red->srcRect.w = red->destRect.w = 16;
	red->srcRect.h = red->destRect.h = 16;

	//config green texture size
	green->srcRect.x = green->srcRect.y = 0;
	green->srcRect.w = green->destRect.w = 16;
	green->srcRect.h = green->destRect.h = 16;

}

void SnakeGame::handleEvents()
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
				if (dir != 0) dir = 3;
				break;
			case SDLK_a:
				if (dir != 2) dir = 1;
				break;
			case SDLK_d:
				if (dir != 1) dir = 2;
				break;
			case SDLK_s:
				if (dir != 3) dir = 0;
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
				break;
			case SDLK_d:
				break;
			case SDLK_s:
				break;
			default:
				break;
			}
		}
	}
}

void SnakeGame::update()
{
	timer += speed;
	if (timer > delay)
	{
		timer = 0;
		Tick();
	}
}

void SnakeGame::render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			white->destRect.x = i * size;
			white->destRect.y = j * size;
			white->draw(renderer);
		}

	for (int i = 0; i < num; i++)
	{
		red->destRect.x = s[i].x * size;
		red->destRect.y = s[i].y * size;
		red->draw(renderer);
	}

	green->destRect.x = f.x * size;
	green->destRect.y = f.y * size;
	green->draw(renderer);
	
	SDL_RenderPresent(renderer);
}

void SnakeGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}