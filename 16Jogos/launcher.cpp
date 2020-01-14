#include "Tetris.h"
#include "DoodleJump.h"
#include "Arkanoid.h"
#include "Snake.h"
#include "Minesweeper.h"
#include "Puzzle.h"
#include "CarRacing.h"
#include "Outrun.h"
#include "Xonix.h"
#include "Bejeweled.h"
#include "NetWalk.h"
#include "Mahjong.h"
#include "Tron.h"
#include "Chess.h"
#include "Volleyball.h"
#include "Asteroids.h"
#include "chooseGame.h"

Game* game = nullptr;

chooseGame* menu;

int gameChoose;

int main(int argc, char* argv[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	menu = new chooseGame();
	menu->init();
	while (menu->running())
	{
		menu->handleEvents();
		menu->update();
		menu->render();
	}

	gameChoose = menu->gameChoose;

	menu->clean();

	switch (gameChoose)
	{
	case (1):
		game = new TetrisGame();
		break;
	case (2):
		game = new DoodleJump();
		break;
	case (3):
		game = new Arkanoid();
		break;
	case (4):
		game = new SnakeGame();
		break;
	case (5):
		game = new Minesweeper();
		break;
	case (6):
		game = new PuzzleGame();
		break;
	case (7):
		game = new CarRacing();
		break;
	case (8):
		game = new Outrun();
		break;
	case (9):
		game = new Xonix();
		break;
	case (10):
		game = new Bejeweled();
		break;
	case (11):
		game = new NetWalk();
		break;
	case (12):
		game = new Mahjong();
		break;
	case (13):
		game = new Tron();
		break;
	case (14):
		game = new Chess();
		break;
	case (15):
		game = new Volleyball();
		break;
	case (16):
		game = new Asteroids();
		break;
	default:
		break;
	}

	if (gameChoose > 0)
	{
		game->init();

		while (game->running())
		{
			frameStart = SDL_GetTicks();

			game->handleEvents();
			game->update();
			game->render();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
		}

		game->clean();
	}

	return 0;
}