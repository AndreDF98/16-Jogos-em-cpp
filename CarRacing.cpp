#include "CarRacing.h"

SDL_Renderer* CarRacing::renderer = nullptr;

SDL_Event CarRacing::event;

const int num =8;
int points[num][2] = {	300, 610,
						1270, 430,
						1380, 2380,
						1900, 2460,
						1970, 1700,
						2550, 1680,
						2560, 3150,
						500, 3300};

struct Car
{
	float x, y, speed, angle;
	int n;

	Car() { speed = 2; angle = 0; n = 0; }

	void move()
	{
		x += sin(angle) * speed;
		y -= cos(angle) * speed;

		float tx = points[n][0];
		float ty = points[n][1];

		float beta = angle - atan2(tx - x, -ty + y);
		if (sin(beta) < 0) angle += 0.08f; else angle -= 0.08f;

		if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) n = (n + 1) % num;
	}
};

const int N = 5;

Car cars[N];

void CarRacing::init(const char* title, int width, int height, bool fullscreen)
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

	car = new Texture();
	car->load(renderer, "assets/car-racing/car.png");
	background = new Texture();
	background->load(renderer, "assets/car-racing/background.png");

	car->srcRect.x = car->srcRect.y = 0;
	car->srcRect.w = car->destRect.w = 43;
	car->srcRect.h = car->destRect.h = 45;
	car->destRect.x = 300;
	car->destRect.y = 220;

	background->srcRect.x = background->srcRect.y = 0;
	background->srcRect.w = 1440;
	background->srcRect.h = 1824;
	background->destRect.w = 1440 * 2;
	background->destRect.h = 1824 * 2;
	background->destRect.x = background->destRect.y = 0;

	for (int i = 0; i < N; i++)
	{
		cars[i].x = float(300 + i * 50);
		cars[i].y = float(1700 + i * 80);
		cars[i].speed = float(7 + i);
	}

}

void CarRacing::handleEvents()
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
				Up = 1;
				break;
			case SDLK_a:
				Left = 1;
				break;
			case SDLK_d:
				Right = 1;
				break;
			case SDLK_s:
				Down = 1;
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
				Up = 0;
				break;
			case SDLK_a:
				Left = 0;
				break;
			case SDLK_d:
				Right = 0;
				break;
			case SDLK_s:
				Down = 0;
				break;
			default:
				break;
			}
		}
	}
}

void CarRacing::update()
{
	if (Up && speed < maxSpeed)
		if (speed < 0) speed += dec;
		else speed += acc;

	if (Down && speed > -maxSpeed)
		if (speed > 0) speed -= dec;
		else speed -= acc;

	if (!Up && !Down)
		if (speed - dec > 0) speed -= dec;
		else if (speed + dec < 0) speed += dec;

	if (Right && speed != 0) angle += turnSpeed * speed / maxSpeed;
	if (Left && speed != 0) angle -= turnSpeed * speed / maxSpeed;

	cars[0].speed = speed;
	cars[0].angle = angle;

	for (int i = 0; i < N; i++) cars[i].move();

	float R = 22;

	//collision
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int dx = cars[i].x - cars[j].x;
			int dy = cars[i].y - cars[j].y;
			if (dx * dx + dy * dy < 4 * R * R)
			{
				cars[i].x += dx / 10;
				cars[i].x += dy / 10;
				cars[j].x -= dx / 10;
				cars[j].y -= dy / 10;
			}
		}

	if (cars[0].x > 320) offsetX = int(cars[0].x - 320);
	if (cars[0].y > 240) offsetY = int(cars[0].y - 240);
}

void CarRacing::render()
{
	SDL_RenderClear(renderer);

	background->destRect.x = -offsetX;
	background->destRect.y = -offsetY;
	background->draw(renderer);

	int colors[10][3] = { {1000, 0, 0}, {0, 1000, 0}, {1000, 0, 1000}, {0, 0, 1000}, {500, 500, 500} };
	for (int i = 0; i < N; i++)
	{
		car->destRect.x = int(cars[i].x - offsetX);
		car->destRect.y = int(cars[i].y - offsetY);
		car->setColor(colors[i][0], colors[i][1], colors[i][2]);
		double a = double(cars[i].angle) * 180 / 3.141592;
		car->draw(renderer, a);
	}

	SDL_RenderPresent(renderer);
}

void CarRacing::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}