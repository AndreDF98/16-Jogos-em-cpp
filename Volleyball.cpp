#include "Volleyball.h"

SDL_Renderer* Volleyball::renderer = nullptr;

SDL_Event Volleyball::event;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

void Volleyball::setWall(int x, int y, int w, int h)
{
	b2PolygonShape gr;
	gr.SetAsBox(w / SCALE, h / SCALE);

	b2BodyDef bdef;
	bdef.position.Set(x / SCALE, y / SCALE);

	b2Body* b_ground = World.CreateBody(&bdef);
	b_ground->CreateFixture(&gr, 1);
}

struct dataStruct
{
	char str[5];
};

dataStruct* ballData = new dataStruct;

dataStruct* player1 = new dataStruct;
dataStruct* player2 = new dataStruct;

void Volleyball::init(const char* title, int width, int height, bool fullscreen)
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
	bg->load(renderer, "assets/volleyball/background.png");
	bg->srcRect.x = bg->srcRect.y = 0;
	bg->destRect.x = bg->destRect.y = 0;
	bg->srcRect.w = bg->destRect.w = 800;
	bg->srcRect.h = bg->destRect.h = 600;

	ball = new Texture();
	ball->load(renderer, "assets/volleyball/ball.png");
	ball->srcRect.x = ball->srcRect.y = 0;
	ball->srcRect.w = ball->destRect.w = 64;
	ball->srcRect.h = ball->destRect.h = 64;

	blobby = new Texture();
	blobby->load(renderer, "assets/volleyball/blobby.png");
	blobby->srcRect.x = blobby->srcRect.y = 0;
	blobby->srcRect.w = blobby->destRect.w = 75;
	blobby->srcRect.h = blobby->destRect.h = 89;

	//box2d
	setWall(400, 520, 2000, 10);
	setWall(370, 415, 5, 170);
	setWall(-30, 0, 0, 2000);
	setWall(790, 0, 10, 2000);

	b2PolygonShape shape;
	shape.SetAsBox(30 / SCALE, 30 / SCALE);
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;

	//players
	for (int i = 0; i < 2; i++)
	{
		bdef.position.Set(float(20 * i), float(2));
		b2CircleShape c;
		c.m_radius = 32 / SCALE;
		c.m_p.Set(0, 13 / SCALE);
		pBody[i] = World.CreateBody(&bdef);
		pBody[i]->CreateFixture(&c, 5);
		c.m_radius = 25 / SCALE;
		c.m_p.Set(0, -20 / SCALE);
		pBody[i]->CreateFixture(&c, 5);
		pBody[i]->SetFixedRotation(true);
	}
	
	pBody[0]->SetUserData(player1);
	pBody[1]->SetUserData(player2);

	//ball
	bdef.position.Set(12, 1);
	b2CircleShape circle;
	circle.m_radius = 32 / SCALE;
	b = World.CreateBody(&bdef);
	b2FixtureDef fdef;
	fdef.shape = &circle;
	fdef.restitution = 0.95f;
	fdef.density = 0.2f;
	b->CreateFixture(&fdef);
	b->SetUserData(ballData);
}

void Volleyball::handleEvents()
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
				pressedButtonsP2[0] = 1;
				break;
			case SDLK_LEFT:
				pressedButtonsP2[1] = 1;
				break;
			case SDLK_RIGHT:
				pressedButtonsP2[2] = 1;
				break;
			case SDLK_DOWN:
				pressedButtonsP2[3] = 1;
				break;
			case SDLK_w:
				pressedButtonsP1[0] = 1;
				break;
			case SDLK_a:
				pressedButtonsP1[1] = 1;
				break;
			case SDLK_d:
				pressedButtonsP1[2] = 1;
				break;
			case SDLK_s:
				pressedButtonsP1[3] = 1;
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
				pressedButtonsP2[0] = 0;
				break;
			case SDLK_LEFT:
				pressedButtonsP2[1] = 0;
				break;
			case SDLK_RIGHT:
				pressedButtonsP2[2] = 0;
				break;
			case SDLK_DOWN:
				pressedButtonsP2[3] = 0;
				break;
			case SDLK_w:
				pressedButtonsP1[0] = 0;
				break;
			case SDLK_a:
				pressedButtonsP1[1] = 0;
				break;
			case SDLK_d:
				pressedButtonsP1[2] = 0;
				break;
			case SDLK_s:
				pressedButtonsP1[3] = 0;
				break;
			default:
				break;
			}
		}
	}
}

void Volleyball::update()
{
	for (int n = 0; n < 2; n++)
		World.Step(1 / 60.f, 8, 3);

	pos = pBody[0]->GetPosition();
	vel = pBody[0]->GetLinearVelocity();

	if (pressedButtonsP1[0]) //up
	{
		if (pos.y * SCALE >= 463) vel.y = -13;
	}
	if (pressedButtonsP1[1]) //left
	{
		vel.x = -8;
	}
	if (pressedButtonsP1[2]) //right
	{
		vel.x = 8;
	}
	if (pressedButtonsP1[3]) //down
	{}

	if (!pressedButtonsP1[2] && !pressedButtonsP1[1]) vel.x = 0;

	pBody[0]->SetLinearVelocity(vel);


	pos = pBody[1]->GetPosition();
	vel = pBody[1]->GetLinearVelocity();

	if (pressedButtonsP2[0]) //w
	{
		if (pos.y * SCALE >= 463) vel.y = -13;
	}
	if (pressedButtonsP2[1]) //a
	{
		vel.x = -8;
	}
	if (pressedButtonsP2[2]) //d
	{
		vel.x = 8;
	}
	if (pressedButtonsP2[3]) //s
	{}

	if (!pressedButtonsP2[2] && !pressedButtonsP2[1]) vel.x = 0;

	pBody[1]->SetLinearVelocity(vel);

	//ball max speed
	vel = b->GetLinearVelocity();
	if (vel.Length() > 15) b->SetLinearVelocity(15 / vel.Length() * vel);
}

void Volleyball::render()
{
	SDL_RenderClear(renderer);

	bg->draw(renderer);

	for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
	{
		b2Vec2 pos = it->GetPosition();
		float angle = it->GetAngle();

		if (it->GetUserData() == player1)
		{
			blobby->destRect.x = int(pos.x * SCALE);
			blobby->destRect.y = int(pos.y * SCALE);
			blobby->setColor(255, 0, 0);

			blobby->draw(renderer, double(angle) * double(DEG));
		}

		if (it->GetUserData() == player2)
		{
			blobby->destRect.x = int(pos.x * SCALE);
			blobby->destRect.y = int(pos.y * SCALE);
			blobby->setColor(0, 255, 0);

			blobby->draw(renderer, double(angle) * double(DEG));
		}

		if (it->GetUserData() == ballData)
		{
			ball->destRect.x = int(pos.x * SCALE);
			ball->destRect.y = int(pos.y * SCALE);

			ball->draw(renderer, double(angle) * double(DEG));
		}
	}

	SDL_RenderPresent(renderer);
}

void Volleyball::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}