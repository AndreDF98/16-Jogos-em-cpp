#include "Asteroids.h"

SDL_Renderer* Asteroids::renderer = nullptr;

SDL_Event Asteroids::event;

bool Asteroids::isCollide(Entity* a, Entity* b)
{
	return	(b->x - a->x) * (b->x - a->x) +
			(b->y - a->y) * (b->y - a->y) <
			(a->R + b->R) * (a->R + b->R);
}

void Asteroids::init(const char* title, int width, int height, bool fullscreen)
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

	spaceship = new Texture();
	spaceship->load(renderer, "assets/asteroids/spaceship.png");
	spaceship->destRect.w = 40;
	spaceship->destRect.h = 40;
	sPlayer = new Animation(spaceship, 40, 0, 40, 40, 1, 0);
	sPlayer_go = new Animation(spaceship, 40, 40, 40, 40, 1, 0);

	p = new player();
	p->settings(sPlayer, 200, 200, 0, 20);
	entities.push_back(p);

	bg = new Texture();
	bg->load(renderer, "assets/asteroids/background.png");
	bg->srcRect.x = bg->srcRect.y = 0;
	bg->destRect.x = bg->destRect.y = 0;
	bg->srcRect.w = bg->destRect.w = 1200;
	bg->srcRect.h = bg->destRect.h = 800;

	explosion = new Texture();
	explosion->load(renderer, "assets/asteroids/explosions/type_A.png");
	explosion->destRect.w = 50;
	explosion->destRect.h = 50;
	sExplosion = new Animation(explosion, 0, 0, 50, 50, 20, 0.5f);

	explosionShip = new Texture();
	explosionShip->load(renderer, "assets/asteroids/explosions/type_A.png");
	explosionShip->destRect.w = 192;
	explosionShip->destRect.h = 192;
	sExplosion_ship = new Animation(explosionShip, 0, 0, 50, 50, 64, 0.5);

	rock = new Texture();
	rock->load(renderer, "assets/asteroids/rock.png");
	rock->destRect.w = 64;
	rock->destRect.h = 64;
	sRock = new Animation(rock, 0, 0, 64, 64, 16, 0.02f);

	for (int i = 0; i < 15; i++)
	{
		asteroid* a = new asteroid();
		a->settings(sRock, rand() % W, rand() % H, float(rand() % 360), 25);
		entities.push_back(a);
	}

	fire = new Texture();
	fire->load(renderer, "assets/asteroids/fire_blue.png");
	fire->destRect.w = 32;
	fire->destRect.h = 64;
	sBullets = new Animation(fire, 0, 0, 32, 64, 16, 0.8f);
}

void Asteroids::handleEvents()
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

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				bullet* bull = new bullet();
				bull->settings(sBullets, p->x, p->y - 10, p->angle, 10);
				entities.push_back(bull);
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

void Asteroids::update()
{
	if (!isPaused)
	{
		Frame += animSpeed;
		if (Frame > frameCount) Frame -= frameCount;
		explosion->srcRect.x = int(Frame) * 50;

		for (auto a : entities)
			for (auto b : entities)
			{

				if (a->name == "asteroid" && b->name == "bullet")
					if (isCollide(a, b))
					{
						a->life = false;
						b->life = false;

						Entity* e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

					}

				if (a->name == "player" && b->name == "asteroid")
					if (isCollide(a, b))
					{
						b->life = false;

						Entity* e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, W / 2, H / 2, 0, 20);
						p->dx = 0; p->dy = 0;
					}
			}
		

		if (pressedButtons[0]) //w
		{
			p->thrust = true;
		}
		else p->thrust = false;
		if (pressedButtons[1]) //a
		{
			p->angle -= 3;
		}
		if (pressedButtons[2]) //d
		{
			p->angle += 3;
		}
		if (pressedButtons[3]) //s
		{}

		if (p->thrust)  p->anim = sPlayer_go;
		else   p->anim = sPlayer;

		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity* e = *i;
			e->update();
			e->anim->update();

			if (e->life == false) { i = entities.erase(i); delete e; }
			else i++;
		}

		for (auto e : entities)
			if (e->name == "explosion")
				if (e->anim->isEnd()) e->life = 0;

		int count = 0;
		for (auto e : entities)
			if (e->name == "asteroid" || e->name == "explosion")
				count++;
		
		if (count == 0)
			isPaused = true;
	}
}

void Asteroids::render()
{
	SDL_RenderClear(renderer);

	bg->draw(renderer);

	for (auto i : entities) i->draw(renderer);

	SDL_RenderPresent(renderer);
}

void Asteroids::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}