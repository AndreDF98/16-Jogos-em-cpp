#include "chooseGame.h"

SDL_Renderer* chooseGame::renderer = nullptr;

SDL_Event chooseGame::event;

bool chooseGame::isIn(Texture* t, Vector2i pos)
{
	SDL_Rect rect;
	SDL_Rect result;

	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = rect.h = 1;

	if (SDL_IntersectRect(&t->destRect, &rect, &result) == SDL_TRUE) return true;
	else return false;
}

void chooseGame::init(const char* title, int width, int height, bool fullscreen)
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
			std::cout << "Menu Window created!" << std::endl;
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


}

void chooseGame::handleEvents()
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
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{

			}
		}
	}
}

void chooseGame::update()
{

}

void chooseGame::render()
{
	SDL_RenderClear(renderer);



	SDL_RenderPresent(renderer);
}

void chooseGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Menu Closed." << std::endl;
}