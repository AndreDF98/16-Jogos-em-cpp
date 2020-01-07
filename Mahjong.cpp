#include "Mahjong.h"

SDL_Renderer* Mahjong::renderer = nullptr;

SDL_Event Mahjong::event;

Vector3i v1 = { NULL, NULL, NULL }; Vector3i v2 = { NULL, NULL, NULL };

int field[50][50][50] = { 0 };

int& f(int x, int y, int z) { return field[y + 2][x + 2][z]; }
int& f(Vector3i v) { return f(v.x, v.y, v.z); }

bool isOpen(int x, int y, int z)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (f(x + 2, y + i, z) > 0 && f(x - 2, y + j, z) > 0) return 0;

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (f(x + i, y + j, z + 1) > 0) return 0;

	return 1;
}

void Mahjong::init(const char* title, int width, int height, bool fullscreen)
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

	tiles = new Texture();
	tiles->load(renderer, "assets/mahjong/tiles.png");
	tiles->srcRect.y = 0;
	tiles->srcRect.w = tiles->destRect.w = w;
	tiles->srcRect.h = tiles->destRect.h = h;

	background = new Texture();
	background->load(renderer, "assets/mahjong/background.png");
	background->srcRect.x = background->srcRect.y = 0;
	background->destRect.x = background->destRect.y = 0;
	background->srcRect.w = background->destRect.w = 858;
	background->srcRect.h = background->destRect.h = 570;

	
	//load from file
	std::fstream myfile("assets/mahjong/map.txt");
	for (int y = 0; y < 18; y++)
		for (int x = 0; x < 30; x++)
		{
			char a; myfile >> a;
			int n = a - '0';
			for (int z = 0; z < n; z++)
				if (f(x - 1, y - 1, z)) f(x - 1, y, z) = f(x, y - 1, z) = 0;
				else f(x, y, z) = 1;
		}
	
	
	//shuffle
	for (int k = 1;; k++)
	{
		std::vector<Vector3i> opens;
		for (int z = 0; z < 10; z++)
			for (int y = 0; y < 18; y++)
				for (int x = 0; x < 30; x++)
					if (f(x, y, z) > 0 && isOpen(x, y, z)) opens.push_back(Vector3i(x, y, z));

		int n = int(opens.size());
		if (n < 2) break;
		int a = 0, b = 0;
		while (a == b) { a = rand() % n; b = rand() % n; }
		f(opens[a]) = -k; if (k > 34) k++;
		f(opens[b]) = -k;
		k %= 42;
	}
	

	for (int z = 0; z < 10; z++)
		for (int y = 0; y < 18; y++)
			for (int x = 0; x < 30; x++) f(x, y, z) *= -1;
}

void Mahjong::handleEvents()
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
				click = true;
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (click)
			{
				for (int z = 0; z < 10; z++)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

					x = int(((x - 30) - z * offX) / stepX);
					y = int((y + z * offY) / stepY);

					for (int i = 0; i < 2; i++)
						for (int j = 0; j < 2; j++)
							if (f(x - i, y - j, z) > 0 && isOpen(x - i, y - j, z))
								v1 = Vector3i(x - i, y - j, z);

					if (v1 == v2) continue;

					int a = f(v1), b = f(v2);
					if (a == b || (a > 34 && a < 39 && b > 34 && b < 39) || (a >= 39 && b >= 39))
					{
						f(v1) *= -1;
						f(v2) *= -1;
					}
					v2 = v1;
				}
			}
			click = false;
		}
	}
}

void Mahjong::update()
{

}

void Mahjong::render()
{
	SDL_RenderClear(renderer);

	background->draw(renderer);

	for (int z = 0; z < 10; z++)
		for (int x = 30; x >= 0; x--)
			for (int y = 0; y < 18; y++)
			{
				int k = f(x, y, z) - 1;
				if (k < 0) continue;
				tiles->srcRect.x = k * w;
				tiles->srcRect.y = 0;
				if (isOpen(x, y, z)) tiles->srcRect.y = h;
				tiles->destRect.x = int(x * stepX + z * offX + 30);
				tiles->destRect.y = int(y * stepY - z * offY);
				tiles->draw(renderer);
			}

	SDL_RenderPresent(renderer);
}

void Mahjong::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}