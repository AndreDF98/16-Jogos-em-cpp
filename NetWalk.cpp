#include "NetWalk.h"

SDL_Renderer* NetWalk::renderer = nullptr;

SDL_Event NetWalk::event;

const int N = 6;

Vector2i Up = { 0, -1 };
Vector2i Down = { 0, 1 };
Vector2i Right = { 1, 0 };
Vector2i Left = { -1, 0 };

Vector2i DIR[4] = { Up, Right, Down, Left };

struct pipe
{
	std::vector<Vector2i> dirs;
	int orientation;
	bool on;
	float angle;

	pipe() { angle = 0; }

	void rotate()
	{
		for (int i = 0; i < dirs.size(); i++)
			if (dirs[i] == Up) dirs[i] = Right;
			else if (dirs[i] == Right) dirs[i] = Down;
			else if (dirs[i] == Down) dirs[i] = Left;
			else if (dirs[i] == Left) dirs[i] = Up;
	}

	bool isConnected(Vector2i dir)
	{
		for (auto d : dirs)
			if (d == dir) return true;
		return false;
	}

} grid[N][N];

pipe& cell(Vector2i v) { return grid[v.x][v.y]; }
bool isOut(Vector2i v)
{
	SDL_Rect a;
	SDL_Rect b;
	SDL_Rect result;

	a.x = a.y = 0;
	a.h = a.w = N;

	b.x = v.x;
	b.y = v.y;
	b.w = b.h = 1;

	if (SDL_IntersectRect(&a, &b, &result) == SDL_TRUE) return false;
	else return true;
}

void generatePuzzle()
{
	std::vector<Vector2i> nodes;
	nodes.push_back(Vector2i(rand() % N, rand() % N));

	while (!nodes.empty())
	{
		int n = rand() % nodes.size();
		Vector2i v = nodes[n];
		Vector2i d = DIR[rand() % 4];

		if (cell(v).dirs.size() == 3) { nodes.erase(nodes.begin() + n); continue; }
		if (cell(v).dirs.size() == 2) if (rand() % 50) continue;

		bool complete = 1;
		for (auto D : DIR)
			if (!isOut(v + D) && cell(v + D).dirs.empty()) complete = 0;
		if (complete) { nodes.erase(nodes.begin() + n); continue; }
		
		if (isOut(v + d)) continue;
		if (!cell(v + d).dirs.empty()) continue;
		cell(v).dirs.push_back(d);
		cell(v + d).dirs.push_back(d * -1);
		nodes.push_back(v + d);
	}
}

void drop(Vector2i v)
{
	if (cell(v).on) return;
	cell(v).on = true;

	for (auto d : DIR)
		if (!isOut(v + d))
			if (cell(v).isConnected(d) && cell(v + d).isConnected(d * -1))
				drop(v + d);
}

void NetWalk::init(const char* title, int width, int height, bool fullscreen)
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
	background->load(renderer, "assets/netwalk/background.png");
	background->srcRect.x = background->srcRect.y = 0;
	background->destRect.x = background->destRect.y = 0;
	background->srcRect.w = background->destRect.w = 390;
	background->srcRect.h = background->destRect.h = 390;

	pipes = new Texture();
	pipes->load(renderer, "assets/netwalk/pipes.png");
	pipes->srcRect.y = 0;
	pipes->srcRect.w = pipes->destRect.w = ts;
	pipes->srcRect.h = pipes->destRect.h = ts;

	computer = new Texture();
	computer->load(renderer, "assets/netwalk/comp.png");
	computer->srcRect.y = 0;
	computer->srcRect.w = computer->destRect.w = 36;
	computer->srcRect.h = computer->destRect.h = 36;

	generatePuzzle();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			pipe& p = grid[j][i];

			for (int n = 4; n > 0; n--)
			{
				std::string s = "";
				for (auto d : DIR) s += p.isConnected(d) ? '1' : '0';
				if (s == "0011" || s == "0111" || s == "0101" || s == "0010") p.orientation = n;
				p.rotate();
			}
		}
}

void NetWalk::handleEvents()
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
				int x, y;
				SDL_GetMouseState(&x, &y);

				int pos[2];
				pos[0] = x + ts / 2 - offset[0];
				pos[1] = y + ts / 2 - offset[1];

				pos[0] /= ts;
				pos[1] /= ts;

				grid[pos[0]][pos[1]].orientation++;
				grid[pos[0]][pos[1]].rotate();

				for (int i = 0; i < N; i++)
					for (int j = 0; j < N; j++)
						grid[j][i].on = 0;

				drop(Vector2i(3, 3));
			}
			click = false;
		}
	}
}

void NetWalk::update()
{
	
}

void NetWalk::render()
{
	SDL_RenderClear(renderer);

	background->draw(renderer);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			pipe& p = grid[j][i];

			int kind = int(p.dirs.size());
			if (kind == 2 && p.dirs[0] == p.dirs[1] * -1) kind = 0;

			p.angle += 5;
			if (p.angle > p.orientation * 90) p.angle = float(p.orientation * 90);

			pipes->srcRect.x = ts * kind;
			pipes->destRect.x = j * ts;
			pipes->destRect.y = i * ts;
			pipes->destRect.x += offset[0] - 27;
			pipes->destRect.y += offset[1] - 27;
			pipes->draw(renderer, p.angle);

			if (kind == 1)
			{
				if (p.on) computer->srcRect.x = 53;
				else computer->srcRect.x = 0;
				computer->destRect.x = j * ts;
				computer->destRect.y = i * ts;
				computer->destRect.x += offset[0] - 27;
				computer->destRect.y += offset[1] - 27;
				computer->draw(renderer);
			}
		}

	SDL_RenderPresent(renderer);
}

void NetWalk::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}