#include "Tron.h"

SDL_Renderer* Tron::renderer = nullptr;

SDL_Event Tron::event;

const int W = 600;
const int H = 480;
int speed = 4;
bool field[W][H] = { 0 };

struct player
{
	int x, y, dir;
	SDL_Color color;
	player(SDL_Color c)
	{
		x = rand() % W;
		y = rand() % H;
		color = c;
		dir = rand() % 4;
	}
	void tick()
	{
		if (dir == 0) y += 1;
		if (dir == 1) x -= 1;
		if (dir == 2) x += 1;
		if (dir == 3) y -= 1;

		if (x >= W) x = 0; if (x < 0) x = W - 1;
		if (y >= H) y = 0; if (y < 0) y = H - 1;
	}
	Vector3i getColor()
	{
		return Vector3i(color.r, color.g, color.b);
	}
};

player p1({ 255, 0, 0 });
player p2({ 0, 255, 0 });

void Tron::drawQuad(SDL_Color color, int x, int y, int w)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = w;
	SDL_RenderFillRect(renderer, &r);
}

void Tron::init(const char* title, int width, int height, bool fullscreen)
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

	bg = new Texture();
	bg->load(renderer, "assets/tron/background.png");
	bg->srcRect.x = bg->srcRect.y = 0;
	bg->destRect.x = bg->destRect.y = 0;
	bg->srcRect.w = bg->destRect.w = 600;
	bg->srcRect.h = bg->destRect.h = 480;
	bg->draw(renderer);

	TTF_Init();

}

void Tron::handleEvents()
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
			case SDLK_UP:
				pressedButtonsP1[0] = 1;
				break;
			case SDLK_LEFT:
				pressedButtonsP1[1] = 1;
				break;
			case SDLK_RIGHT:
				pressedButtonsP1[2] = 1;
				break;
			case SDLK_DOWN:
				pressedButtonsP1[3] = 1;
				break;
			case SDLK_w:
				pressedButtonsP2[0] = 1;
				break;
			case SDLK_a:
				pressedButtonsP2[1] = 1;
				break;
			case SDLK_d:
				pressedButtonsP2[2] = 1;
				break;
			case SDLK_s:
				pressedButtonsP2[3] = 1;
				break;
			default:
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				pressedButtonsP1[0] = 0;
				break;
			case SDLK_LEFT:
				pressedButtonsP1[1] = 0;
				break;
			case SDLK_RIGHT:
				pressedButtonsP1[2] = 0;
				break;
			case SDLK_DOWN:
				pressedButtonsP1[3] = 0;
				break;
			case SDLK_w:
				pressedButtonsP2[0] = 0;
				break;
			case SDLK_a:
				pressedButtonsP2[1] = 0;
				break;
			case SDLK_d:
				pressedButtonsP2[2] = 0;
				break;
			case SDLK_s:
				pressedButtonsP2[3] = 0;
				break;
			default:
				break;
			}
		}
	}
}

void Tron::update()
{
	if (pressedButtonsP1[0]) //up
	{
		if (p1.dir != 0) p1.dir = 3;
	}
	if (pressedButtonsP1[1]) //left
	{
		if (p1.dir != 2) p1.dir = 1;
	}
	if (pressedButtonsP1[2]) //right
	{
		if (p1.dir != 1) p1.dir = 2;
	}
	if (pressedButtonsP1[3]) //down
	{
		if (p1.dir != 3) p1.dir = 0;
	}

	if (pressedButtonsP2[0]) //w
	{
		if (p2.dir != 0) p2.dir = 3;
	}
	if (pressedButtonsP2[1]) //a
	{
		if (p2.dir != 2) p2.dir = 1;
	}
	if (pressedButtonsP2[2]) //d
	{
		if (p2.dir != 1) p2.dir = 2;
	}
	if (pressedButtonsP2[3]) //s
	{
		if (p2.dir != 3) p2.dir = 0;
	}

}

void Tron::render()
{
	if (isPaused) return;

	int winner = 0;

	for (int i = 0; i < speed; i++)
	{
		p1.tick(); p2.tick();
		if (field[p1.x][p1.y] == 1) { isPaused = true; winner = 2; }
		if (field[p2.x][p2.y] == 1) { isPaused = true; winner = 1; }

		if (isPaused)
		{
			Texture* tex = new Texture();
			SDL_Color color;
			if (winner == 1) color = p1.color;
			else color = p2.color;
			tex->loadFont(renderer, "assets/tron/arial.ttf", 35, "You Win!", color);
			tex->srcRect.x = tex->srcRect.y = 0;
			tex->destRect.x = W / 2 - 60;
			tex->destRect.y = 20;
			tex->draw(renderer);
		}

		field[p1.x][p1.y] = 1;
		field[p2.x][p2.y] = 1;

		drawQuad(p1.color, p1.x, p1.y, 5);
		drawQuad(p2.color, p2.x, p2.y, 5);
		SDL_RenderPresent(renderer);
	}

	SDL_RenderPresent(renderer);
}

void Tron::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}