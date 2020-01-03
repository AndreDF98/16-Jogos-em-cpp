#include "Outrun.h"

SDL_Renderer* Outrun::renderer = nullptr;

SDL_Event Outrun::event;

int width = 1024, height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84f; //camera depth

struct Line
{
	float x, y, z;
	int X, Y, W;
	float scale, curve, spriteX, clip;
	bool hasSprite;
	Texture* tree;

	Line()
	{
		hasSprite = curve = x = y = z = 0;
		
	}

	//from world to screen coordinates
	void project(int camX, int camY, int camZ)
	{
		scale = camD / (z - camZ);
		X = int((1 + scale * (x - camX)) * width / 2);
		Y = int((1 - scale * (y - camY)) * height / 2);
		W = int(scale * roadW * width / 2);
	}

	void drawSprite(SDL_Renderer* renderer)
	{
		if (hasSprite)
		{

			int w = tree->srcRect.w;
			int h = tree->srcRect.h;

			float destX = X + scale * spriteX * width / 2;
			float destY = float(Y + 4);
			float destW = float(w * W / 266);
			float destH = float(h * W / 266);

			destX += destW * spriteX; //offset X
			destY += destH * (-1); //offset Y

			float clipH = destY + destH - clip;
			if (clipH < 0) clipH = 0;
			if (clipH >= destH) return;
			
			tree->destRect.w = int(tree->srcRect.w * destW / w);
			tree->destRect.h = int(tree->srcRect.h * destH / h);
			tree->destRect.x = int(destX);
			tree->destRect.y = int(destY);
			tree->draw(renderer);
			
		}
		
	}
};

std::vector<Line> lines;
int N;
int pos = 0;
int startPos;
int playerX = 0;

Texture* bg;
Texture* tree;

void Outrun::drawQuad(int* color, int x1, int y1, int w1, int x2, int y2, int w2)
{
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
	SDL_Rect r;
	r.x = x2 - (w2 / 2);
	r.y = y2;
	r.w = w2;
	r.h = y1 - y2;
	SDL_RenderFillRect(renderer, &r);
}

void Outrun::init(const char* title, int width, int height, bool fullscreen)
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

	bg = new Texture();
	bg->load(renderer, "assets/outrun/bg.png");
	bg->srcRect.x = bg->srcRect.y = 0;
	bg->destRect.x = bg->srcRect.y = 0;
	bg->srcRect.w = bg->destRect.w = 1024;
	bg->srcRect.h = bg->destRect.h = 411;

	tree = new Texture();
	tree->load(Outrun::renderer, "assets/outrun/5.png");
	tree->srcRect.x = tree->srcRect.y = 0;
	tree->srcRect.w = 500;
	tree->srcRect.h = 520;

	for (int i = 0; i < 1600; i++)
	{
		Line line;
		line.z = float(i * segL);

		if (i > 300 && i < 700) line.curve = 0.5;

		if (i > 750) line.y = float(abs(sin(i / 30) * 1500));

		if (i % 20 == 0) { line.spriteX = -2.5; line.hasSprite = 1; line.tree = tree; }

		lines.push_back(line);
	}

	N = int(lines.size());

}

void Outrun::handleEvents()
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

void Outrun::update()
{
	if (pressedButtons[0]) //w
	{
		pos += 200;
	}
	if (pressedButtons[1]) //a
	{
		playerX -= 200;
	}
	if (pressedButtons[2]) //d
	{
		playerX += 200;
	}
	if (pressedButtons[3]) //s
	{
		pos -= 200;
	}

	while (pos >= N * segL) pos -= N * segL;
	while (pos < 0) pos += N * segL;

	startPos = (pos / segL) % N;
}

void Outrun::render()
{
	SDL_RenderClear(renderer);

	bg->draw(renderer);

	int camH = int(1500 + lines[startPos].y);
	int maxy = height;
	float x = 0, dx = 0;

	for (int n = startPos; n < startPos + 300; n++)
	{
		Line &l = lines[abs(n) % N];
		l.project(int(playerX - x), camH, pos - (n>=N?N*segL:0));
		x += dx;
		dx += l.curve;

		l.clip = float(maxy);
		if (l.Y >= maxy) continue;
		if (n != startPos) maxy = l.Y;
		
		int grass[3], rumble[3], road[3];
		if ((n / 3) % 2)
		{
			grass[0] = 16; grass[1] = 200; grass[2] = 16;
			rumble[0] = 255; rumble[1] = 255; rumble[2] = 255;
			road[0] = 107; road[1] = 107; road[2] = 107;
		}
		else
		{
			grass[0] = 0; grass[1] = 154; grass[2] = 0;
			rumble[0] = 0; rumble[1] = 0; rumble[2] = 0;
			road[0] = 105; road[1] = 105; road[2] = 105;
		}

		Line p = lines[abs(n - 1) % N];
		drawQuad(grass, 0, p.Y, width, 0, l.Y, 2*width);
		drawQuad(rumble, p.X, p.Y, int(p.W * 1.2f), l.X, l.Y, int(l.W * 1.2f));
		drawQuad(road, p.X, p.Y, p.W, l.X, l.Y, l.W);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	for (int n = startPos + 300; n > startPos; n--)
		lines[n % N].drawSprite(renderer);

	SDL_RenderPresent(renderer);
}

void Outrun::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}