#include "Bejeweled.h"

SDL_Renderer* Bejeweled::renderer = nullptr;

SDL_Event Bejeweled::event;

struct piece
{
	int x, y, col, row, kind, match, alpha;
	piece() { match = 0; alpha = 255; }
} grid[10][10];

void swap(piece p1, piece p2)
{
	std::swap(p1.col, p2.col);
	std::swap(p1.row, p2.row);

	grid[p1.row][p1.col] = p1;
	grid[p2.row][p2.col] = p2;
}

void Bejeweled::init(const char* title, int width, int height, bool fullscreen)
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

	background = new Texture();
	background->load(renderer, "assets/bejeweled/background.png");
	background->srcRect.x = background->srcRect.y = 0;
	background->destRect.x = background->destRect.y = 0;
	background->srcRect.w = background->destRect.w = 740;
	background->srcRect.h = background->destRect.h = 480;

	gems = new Texture();
	gems->load(renderer, "assets/bejeweled/gems.png");
	gems->srcRect.y = 0;
	gems->srcRect.w = gems->destRect.w = 49;
	gems->srcRect.h = gems->destRect.h = 49;

	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			grid[i][j].kind = rand() % 7;
			grid[i][j].col = j;
			grid[i][j].row = i;
			grid[i][j].x = j * ts;
			grid[i][j].y = i * ts;
		}

}

void Bejeweled::handleEvents()
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
				if (!isSwap && !isMoving) click++;
				int x, y;
				SDL_GetMouseState(&x, &y);
				pos[0] = x - offset[0];
				pos[1] = y - offset[1];
			}
		}
	}
}

void Bejeweled::update()
{
	if (click == 1)
	{
		x0 = pos[0] / ts + 1;
		y0 = pos[1] / ts + 1;
	}
	if (click == 2)
	{
		x = pos[0] / ts + 1;
		y = pos[1] / ts + 1;
		if (abs(x - x0) + abs(y - y0) == 1)
		{
			swap(grid[y0][x0], grid[y][x]);
			isSwap = 1;
			click = 0;
		}
		else click = 1;
	}

	//match finding
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			if (grid[i][j].kind == grid[i + 1][j].kind)
				if (grid[i][j].kind == grid[i - 1][j].kind)
					for (int n = -1; n <= 1; n++) grid[i + n][j].match++;

			if (grid[i][j].kind == grid[i][j + 1].kind)
				if (grid[i][j].kind == grid[i][j - 1].kind)
					for (int n = -1; n <= 1; n++) grid[i][j + n].match++;
		}

	//moving animation
	isMoving = false;
	for (int i =1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			piece& p = grid[i][j];
			int dx, dy;
			for (int n = 0; n < 4; n++)
			{
				dx = p.x - p.col * ts;
				dy = p.y - p.row * ts;
				if (dx) p.x -= dx / abs(dx);
				if (dy) p.y -= dy / abs(dy);
				if (dx || dy) isMoving = 1;
			}
		}

	//deleting animation
	if (!isMoving)
		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
				if (grid[i][j].match) if (grid[i][j].alpha > 10) { grid[i][j].alpha -= 10; isMoving = true; }

	//get score
	int score = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			score += grid[i][j].match;

	//second swap if no match
	if (isSwap && !isMoving)
	{
		swap(grid[y0][x0], grid[y][x]);
		isSwap = 0;
	}

	//update grid
	if (!isMoving)
	{
		for (int i = 8; i > 0; i--)
			for (int j = 1; j <= 8; j++)
				if (grid[i][j].match)
					for (int n = i; n > 0; n--)
						if (!grid[n][j].match)
						{
							swap(grid[n][j], grid[i][j]);
							break;
						}

		for (int j = 1; j <= 8; j++)
			for (int i = 8, n = 0; i > 0; i--)
				if (grid[i][j].match)
				{
					grid[i][j].kind = rand() % 7;
					grid[i][j].y = -ts * n++;
					grid[i][j].match = 0;
					grid[i][j].alpha = 255;
				}
	}
}

void Bejeweled::render()
{
	SDL_RenderClear(renderer);

	background->draw(renderer);

	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			piece p = grid[i][j];
			gems->srcRect.x = p.kind * 49;
			gems->setAlpha(p.alpha);
			gems->destRect.x = p.x;
			gems->destRect.y = p.y;
			gems->destRect.x += offset[0] - ts;
			gems->destRect.y += offset[1] - ts;
			gems->draw(renderer);
		}

	SDL_RenderPresent(renderer);
}

void Bejeweled::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}