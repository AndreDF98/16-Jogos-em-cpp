#include "Puzzle.h"

#include <algorithm>

SDL_Renderer* PuzzleGame::renderer = nullptr;

SDL_Event PuzzleGame::event;

void PuzzleGame::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("15-Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
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

	int pos[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::random_shuffle(pos, pos + 15);

	int n = 0;
	//load and config tiles texture
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			n++;
			tiles[n] = new Texture();
			tiles[n]->load(renderer, "assets/puzzle/15.png");
			tiles[n]->srcRect.y = i *w;
			tiles[n]->srcRect.x = j * w;
			tiles[n]->srcRect.w = tiles[n]->srcRect.h = w;
			tiles[n]->destRect.w = tiles[n]->destRect.h = w;
			grid[i + 1][j + 1] = pos[n-1];
		}
	grid[4][4] = 16;
}

void PuzzleGame::handleEvents()
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
				int x, y;
				SDL_GetMouseState(&x, &y);

				x = x / w + 1;
				y = y / w + 1;

				int dx = 0, dy = 0;

				if (grid[x + 1][y] == 16) { dx = 1; dy = 0; }
				if (grid[x][y + 1] == 16) { dx = 0; dy = 1; }
				if (grid[x][y - 1] == 16) { dx = 0; dy = -1; }
				if (grid[x - 1][y] == 16) { dx = -1; dy = 0; }

				int n = grid[x][y];
				grid[x][y] = 16;
				grid[x + dx][y + dy] = n;

				//animation
				tiles[16]->destRect.x += -dx * w;
				tiles[16]->destRect.y += -dy * w;
				int speed = 1;

				for (int i = 0; i < w; i += speed)
				{
					tiles[n]->destRect.x += speed * dx;
					tiles[n]->destRect.y += speed * dy;
					tiles[16]->draw(renderer);
					tiles[n]->draw(renderer);
					SDL_RenderPresent(renderer);
				}
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{

			}
		}
	}
}

void PuzzleGame::update()
{

}

void PuzzleGame::render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			int n = grid[i + 1][j + 1];
			tiles[n]->destRect.x = i * w;
			tiles[n]->destRect.y = j * w;
			tiles[n]->draw(renderer);
		}

	SDL_RenderPresent(renderer);
}

void PuzzleGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}