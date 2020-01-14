#include "Minesweeper.h"

SDL_Renderer* Minesweeper::renderer = nullptr;

SDL_Event Minesweeper::event;

void Minesweeper::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, 0);
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

	//load texture
	tiles = new Texture();
	tiles->load(renderer, "assets/minesweeper/tiles.png");

	//config tiles texture size
	tiles->srcRect.w = tiles->destRect.w = 32;
	tiles->srcRect.h = tiles->destRect.h = 32;
	tiles->srcRect.y = 0;

	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			sgrid[i][j] = 10;
			if (rand() % 8 == 0) grid[i][j] = 9;
			else grid[i][j] = 0;
		}

	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			int n = 0;
			if (grid[i][j] == 9) continue;
			if (grid[i + 1][j] == 9) n++;
			if (grid[i][j + 1] == 9) n++;
			if (grid[i - 1][j] == 9) n++;
			if (grid[i][j - 1] == 9) n++;
			if (grid[i + 1][j + 1] == 9) n++;
			if (grid[i - 1][j - 1] == 9) n++;
			if (grid[i - 1][j + 1] == 9) n++;
			if (grid[i + 1][j - 1] == 9) n++;
			grid[i][j] = n;
		}

}

void Minesweeper::clickTile(int i, int j)
{
	if (grid[i][j] == 0 && sgrid[i][j] != 0)
	{
		sgrid[i][j] = grid[i][j];
		clickTile(i + 1, j);
		clickTile(i, j + 1);
		clickTile(i - 1, j);
		clickTile(i, j - 1);
	}
	else sgrid[i][j] = grid[i][j];
}

void Minesweeper::handleEvents()
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
				clickTile(x, y);
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) sgrid[x][y] = 11;		
		}
	}
}

void Minesweeper::update()
{
	SDL_GetMouseState(&x, &y);

	x = x / w;
	y = y / w;
}

void Minesweeper::render()
{
	SDL_RenderClear(renderer);

	for (int i = 1; i <=10; i++)
		for (int j = 1; j <= 10; j++)
		{
			if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];
			tiles->srcRect.x = sgrid[i][j] * w;
			tiles->destRect.x = i * w;
			tiles->destRect.y = j * w;
			tiles->draw(renderer);
		}

	SDL_RenderPresent(renderer);
}

void Minesweeper::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}