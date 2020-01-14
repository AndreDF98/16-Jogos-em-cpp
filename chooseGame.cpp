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

	bg = new Texture();
	bg->load(renderer, "assets/menu/background.png");
	bg->srcRect.x =bg->srcRect.y = 0;
	bg->destRect.x = bg->destRect.y = 0;
	bg->srcRect.w =bg->destRect.w = 720;
	bg->srcRect.h =bg->destRect.h = 625;

	std::string temp;

	for (int i = 0; i < 16; i++)
	{
		button[i] = new Texture();
		
		temp.append("assets/menu/button");
		temp.append(std::to_string(i + 1));
		temp.append(".png");
		const char* path = temp.c_str();
		button[i]->load(renderer, path);

		button[i]->srcRect.x = button[i]->srcRect.y = 0;
		button[i]->srcRect.w = button[i]->destRect.w = 200;
		button[i]->srcRect.h = button[i]->destRect.h = 60;

		button[i]->setAlpha(0);
		button[i]->setColor(120, 120, 120);

		temp.clear();
	}

	for (int i = 0; i < 6; i++)
	{
		button[i]->destRect.x = 45;
		button[i]->destRect.y = (i+1) * (60 + 15) + 70;
	}

	for (int i = 0; i < 4; i++)
	{
		button[i + 6]->destRect.x = 45 + 200 + 15;
		button[i + 6]->destRect.y = (i + 1) * (60 + 15) + 70 + 60 * 2 + 15 * 2;
	}

	for (int i = 0; i < 6; i++)
	{
		button[i + 10]->destRect.x = 45 + 200 * 2 + 15 * 2;
		button[i + 10]->destRect.y = (i + 1) * (60 + 15) + 70;
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
				for (int i = 0; i < 16; i++)
					if (isIn(button[i], pos))
					{
						gameChoose = i + 1;
						isRunning = false;
					}
			}
		}
	}
}

void chooseGame::update()
{
	SDL_GetMouseState(&pos.x, &pos.y);

	for (int i = 0; i < 16; i++)
		if (isIn(button[i], pos))
			button[i]->setColor(255, 255, 255);
		else button[i]->setColor(120, 120, 120);
}

void chooseGame::render()
{
	SDL_RenderClear(renderer);

	bg->draw(renderer);

	for (int i = 0; i < 16; i++) button[i]->draw(renderer);

	SDL_RenderPresent(renderer);
}

void chooseGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Menu Closed." << std::endl << std::endl;
}