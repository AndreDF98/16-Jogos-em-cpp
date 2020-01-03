#include "Xonix.h"

SDL_Renderer* Xonix::renderer = nullptr;

SDL_Event Xonix::event;

const int M = 25;
const int N = 40;

int ts = 18; //tile size

int grid[M][N] = { 0 };

struct Enemy
{
	int x, y, dx, dy;

	Enemy()
	{
		x = y = 300;
		dx = 4 - rand() % 8;
		dy = 4 - rand() % 8;
	}

	void move()
	{
		x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
		y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
	}
};
Enemy a[10];

void drop(int y, int x)
{
	if (grid[y][x] == 0) grid[y][x] = -1;
	if (grid[y - 1][x] == 0) drop(y - 1, x);
	if (grid[y + 1][x] == 0) drop(y + 1, x);
	if (grid[y][x - 1] == 0) drop(y, x - 1);
	if (grid[y][x + 1] == 0) drop(y, x + 1);
}

void Xonix::init(const char* title, int width, int height, bool fullscreen)
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
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
		isPaused = false;
	}
	else
	{
		isRunning = false;
	}

	tiles = new Texture();
	tiles->load(renderer, "assets/xonix/tiles.png");
	tiles->srcRect.y = 0;
	tiles->srcRect.w = tiles->destRect.w = 18;
	tiles->srcRect.h = tiles->destRect.h = 18;

	gameOver = new Texture();
	gameOver->load(renderer, "assets/xonix/gameover.png");
	gameOver->srcRect.x = gameOver->srcRect.y = 0;
	gameOver->srcRect.w = gameOver->destRect.w = 500;
	gameOver->srcRect.h = gameOver->destRect.h = 200;
	gameOver->destRect.x = gameOver->destRect.y = 100;

	enemy = new Texture();
	enemy->load(renderer, "assets/xonix/enemy.png");
	enemy->srcRect.x = enemy->srcRect.y = 0;
	enemy->srcRect.w = enemy->destRect.w = 40;
	enemy->srcRect.h = enemy->destRect.h = 40;

	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			if ((i == 0) || (j == 0) || (i == M - 1) || (j == N - 1)) grid[i][j] = 1;
}

void Xonix::handleEvents()
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
				pressedButtons[0] = 1;
				break;
			case SDLK_a:
				pressedButtons[1] = 1;
				break;
			case SDLK_d:
				pressedButtons[2] = 1;
				break;
			case SDLK_s:
				pressedButtons[3] = 1;
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
				pressedButtons[0] = 0;
				break;
			case SDLK_a:
				pressedButtons[1] = 0;
				break;
			case SDLK_d:
				pressedButtons[2] = 0;
				break;
			case SDLK_s:
				pressedButtons[3] = 0;
				break;
			default:
				break;
			}
		}
	}
}

void Xonix::update()
{
	if (!isPaused)
	{
		if (pressedButtons[0]) //w
		{
			dx = 0;
			dy = -1;
		}
		if (pressedButtons[1]) //a
		{
			dx = -1;
			dy = 0;
		}
		if (pressedButtons[2]) //d
		{
			dx = 1;
			dy = 0;
		}
		if (pressedButtons[3]) //s
		{
			dx = 0;
			dy = 1;
		}

		timer += speed;
		if (timer > delay)
		{
			x += dx;
			y += dy;

			if (x < 0) x = 0; if (x > N - 1) x = N - 1;
			if (y < 0) y = 0; if (y > M - 1) y = M - 1;

			if (grid[y][x] == 2) isPaused = true;
			if (grid[y][x] == 0) grid[y][x] = 2;
			timer = 0;
		}

		for (int i = 0; i < enemyCount; i++) a[i].move();

		if (grid[y][x] == 1)
		{
			dx = dy = 0;

			for (int i = 0; i < enemyCount; i++)
				drop(a[i].y / ts, a[i].x / ts);

			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
					if (grid[i][j] == -1) grid[i][j] = 0;
					else grid[i][j] = 1;
		}

		for (int i = 0; i < enemyCount; i++)
			if (grid[a[i].y / ts][a[i].x / ts] == 2) isPaused = true;
	}
	
}

void Xonix::render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			if (grid[i][j] == 0) continue;
			if (grid[i][j] == 1) tiles->srcRect.x = 0;
			if (grid[i][j] == 2) tiles->srcRect.x = 54;
			tiles->destRect.x = j * ts;
			tiles->destRect.y = i * ts;
			tiles->draw(renderer);
		}

	tiles->srcRect.x = 36;
	tiles->destRect.x = x * ts;
	tiles->destRect.y = y * ts;
	tiles->draw(renderer);

	enemyAngle += 10;
	double x = enemyAngle % 360;
	for (int i = 0; i < enemyCount; i++)
	{
		enemy->destRect.x = a[i].x - ts;
		enemy->destRect.y = a[i].y - ts;
		enemy->draw(renderer, x);
	}

	if (isPaused) gameOver->draw(renderer);

	SDL_RenderPresent(renderer);
}

void Xonix::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}