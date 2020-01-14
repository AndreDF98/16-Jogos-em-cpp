#include "Chess.h"

SDL_Renderer* Chess::renderer = nullptr;

SDL_Event Chess::event;

bool Chess::collide(Texture* t, Vector2i pos)
{
	SDL_Rect rect;
	SDL_Rect result;

	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = rect.h = 1;

	if (SDL_IntersectRect(&t->destRect, &rect, &result) == SDL_TRUE) return true;
	else return false;
}

void Chess::loadPosition()
{
	int k = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int n = board[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			
			figures[k]->srcRect.x = size * x;
			figures[k]->srcRect.y = size * y;

			figures[k]->destRect.x = size * j;
			figures[k]->destRect.y = size * i;
			k++;
		}

	for (int i = 0; i < position.length(); i += 5)
		move(position.substr(i, 4));
}

std::string Chess::toChessNote(Vector2i p)
{
	std::string s = "";
	s += char(p.x / size + 97);
	s += char(7 - p.y / size + 49);
	return s;
}

Vector2i Chess::toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return Vector2i(x * size, y * size);
}

void Chess::move(std::string str)
{
	Vector2i oldPos = toCoord(str[0], str[1]);
	Vector2i newPos = toCoord(str[2], str[3]);

	for (int i = 0; i < 32; i++)
		if (Vector2i(figures[i]->destRect.x, figures[i]->destRect.y) == newPos) { figures[i]->destRect.x = -100; figures[i]->destRect.y = -100; }

	for (int i = 0; i < 32; i++)
		if (Vector2i(figures[i]->destRect.x, figures[i]->destRect.y) == oldPos) { figures[i]->destRect.x = newPos.x; figures[i]->destRect.y = newPos.y; }
}

void Chess::init()
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 453, 453, 0);
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
		isPaused = false;
	}
	else
	{
		isRunning = false;
	}

	table = new Texture();
	table->load(renderer, "assets/chess/board0.png");
	table->srcRect.x = table->srcRect.y = 0;
	table->destRect.x = table->destRect.y = 0;
	table->srcRect.w = table->destRect.w = 453;
	table->srcRect.h = table->destRect.h = 453;

	for (int i = 0; i < 32; i++)
	{
		figures[i] = new Texture();
		figures[i]->load(renderer, "assets/chess/figures.png");
		figures[i]->srcRect.w = figures[i]->destRect.w = size;
		figures[i]->srcRect.h = figures[i]->destRect.h = size;
	}

	loadPosition();

}

void Chess::handleEvents()
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
			if (event.key.keysym.sym == SDLK_BACKSPACE)
			{
				if (position.length() > 6) position.erase(position.length() - 6, 5);
				loadPosition();
			}

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				str = "d7d5"; //for example

				oldPos = toCoord(str[0], str[1]);
				newPos = toCoord(str[2], str[3]);

				for (int i = 0; i < 32; i++) if (Vector2i(figures[i]->destRect.x, figures[i]->destRect.y) == oldPos) n = i;

				//animation
				for (int k = 0; k < 50; k++)
				{
					Vector2i p = newPos - oldPos;
					figures[n]->destRect.x += p.x / 50;
					figures[n]->destRect.y -= p.y / 50;
					table->draw(renderer);
					for (int i = 0; i < 32; i++)
						figures[i]->draw(renderer);
					figures[n]->draw(renderer);
					SDL_RenderPresent(renderer);
				}

				move(str); position += str + " ";
				figures[n]->destRect.x = newPos.x;
				figures[n]->destRect.y = newPos.y;
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				for (int i = 0; i < 32; i++)
				if (collide(figures[i], pos))
				{
					isMove = true; n = i;
					dx = pos.x - figures[i]->destRect.x;
					dy = pos.y - figures[i]->destRect.y;
					oldPos = { figures[i]->destRect.x, figures[i]->destRect.y };
				}
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				isMove = false;
				Vector2i p = {figures[n]->destRect.x + size / 2, figures[n]->destRect.y + size / 2 };
				newPos = Vector2i(size * int(p.x / size), size * int(p.y / size));
				str = toChessNote(oldPos) + toChessNote(newPos);
				move(str);
				position += str + " ";
				std::cout << str << std::endl;
				figures[n]->destRect.x = newPos.x;
				figures[n]->destRect.y = newPos.y;
			}
		}
	}
}

void Chess::update()
{
	SDL_GetMouseState(&pos.x, &pos.y);

	if (isMove)
	{
		figures[n]->destRect.x = pos.x - dx;
		figures[n]->destRect.y = pos.y - dy;
	}
	//std::cout << isMove << std::endl;
}

void Chess::render()
{
	SDL_RenderClear(renderer);

	table->draw(renderer);

	for (int i = 0; i < 32; i++) figures[i]->draw(renderer);

	SDL_RenderPresent(renderer);
}

void Chess::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}