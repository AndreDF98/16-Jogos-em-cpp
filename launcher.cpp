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

Game* game = nullptr;

int gameChoose;

int main(int argc, char* argv[])
{
	gameChoose = 14;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	switch (gameChoose)
	{
	case (1):
		game = new TetrisGame();
		game->init("Tetris", 320, 480, false);
		break;
	case (2):
		game = new DoodleJump();
		game->init("Doodle Jump", 400, 533, false);
		break;
	case (3):
		game = new Arkanoid();
		game->init("Arkanoid", 520, 450, false);
		break;
	case (4):
		game = new SnakeGame();
		game->init("Snake", 480, 320, false);
		break;
	case (5):
		game = new Minesweeper();
		game->init("Minesweeper", 400, 400, false);
		break;
	case (6):
		game = new PuzzleGame();
		game->init("15-Puzzle", 256, 256, false);
		break;
	case (7):
		game = new CarRacing();
		game->init("Car Racing", 640, 480, false);
		break;
	case (8):
		game = new Outrun();
		game->init("Outrun Racing", 1024, 768, false);
		break;
	case (9):
		game = new Xonix();
		game->init("Xonix", 720, 450, false);
		break;
	case (10):
		game = new Bejeweled();
		game->init("Bejeweled", 740, 480, false);
		break;
	case (11):
		game = new NetWalk();
		game->init("NetWalk", 390, 390, false);
		break;
	case (12):
		game = new Mahjong();
		game->init("Mahjong", 740, 570, false);
		break;
	case (13):
		game = new Tron();
		game->init("Tron", 600, 480, false);
		break;
	case (14):
		game = new Chess();
		game->init("Chess", 453, 453, false);
		break;
	default:
		break;
	}
	

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

	return 0;
}