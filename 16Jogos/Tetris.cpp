#include "Tetris.h"

SDL_Renderer* TetrisGame::renderer = nullptr;

SDL_Event TetrisGame::event;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{ int x, y; } a[4], b[4];

int figures[7][4] =
{
	{1, 3, 5, 7 },
	{2, 4, 5, 7 },
	{3, 5, 4, 6 },
	{3, 5, 4, 7 },
	{2, 3, 5, 7 },
	{3, 5, 7, 6 },
	{2, 3, 4, 5 }
};

bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	}
	return 1;
}

void TetrisGame::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 480, 0);
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

	//load music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("assets/tetris/Tetris.wav");
	Mix_PlayMusic(music, -1);

	//load textures
	tileTex = new Texture();
	tileTex->load(renderer, "assets/tetris/tiles.png");
	bgTex = new Texture();
	bgTex->load(renderer, "assets/tetris/background.png");
	frameTex = new Texture();
	frameTex->load(renderer, "assets/tetris/frame.png");

	//config tile texture size
	tileTex->srcRect.x =	tileTex->srcRect.y = 0;
	tileTex->srcRect.w =	18;
	tileTex->srcRect.h =	18;
	tileTex->destRect.w =	18;
	tileTex->destRect.h =	18;

	//config bg texture size
	bgTex->srcRect.x =	bgTex->srcRect.y = 0;
	bgTex->destRect.x = bgTex->destRect.y = 0;
	bgTex->srcRect.w =	bgTex->destRect.w = 320;
	bgTex->srcRect.h =	bgTex->destRect.h = 480;

	//config frame texture size
	frameTex->srcRect.x =	frameTex->srcRect.y = 0;
	frameTex->destRect.x =	frameTex->destRect.y = 0;
	frameTex->srcRect.w =	frameTex->destRect.w = 320;
	frameTex->srcRect.h =	frameTex->destRect.h = 480;

	//first drop
	colorNum = 1 + rand() % 7;
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

}

void TetrisGame::handleEvents()
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
				rotate = true;
				break;
			case SDLK_a:
				dx = -1;
				break;
			case SDLK_d:
				dx = 1;
				break;
			case SDLK_s:
				delay = 0.05f;
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
				delay = 0.3f;
				break;
			default:
				break;
			}
		}
	}
}

void TetrisGame::update()
{
	if (isPaused == false)
	{
		// move
		for (int i = 0; i < 4; i++) { b[i] = a[i];  a[i].x += dx; }
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

		//rotate
		if (rotate)
		{
			Point p = a[1];
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		}

		timer += speed;
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];  a[i].y += 1;
				if (field[b[i].y][b[i].x] != 0)
				{
					std::cout << "Game Over!!" << std::endl;
					Mix_PauseMusic();
					isPaused = true;
				}

			}

			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;


				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

		dx = 0; rotate = 0;
	}
}

void TetrisGame::render()
{
	SDL_RenderClear(renderer);

	bgTex->draw(renderer);

	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 0) continue;
			tileTex->srcRect.x = field[i][j] * 18;
			tileTex->destRect.x = j * 18;
			tileTex->destRect.y = i * 18;
			tileTex->destRect.x += 28; tileTex->destRect.y += 31; //offset
			tileTex->draw(renderer);
		}

	for (int i = 0; i < 4; i++)
	{
		tileTex->srcRect.x = colorNum * 18;
		tileTex->destRect.x = a[i].x * 18;
		tileTex->destRect.y = a[i].y * 18;
		tileTex->destRect.x += 28; tileTex->destRect.y += 31; //offset
		tileTex->draw(renderer);
	}

	frameTex->draw(renderer);

	SDL_RenderPresent(renderer);
}

void TetrisGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}