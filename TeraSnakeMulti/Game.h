#pragma once
#include "client.h"
#include <Grid.h>
#include "player.h"
#include <Window.h>

class Game
{
public:
	Game(Window* mainWindow);
	~Game();
	int loop();
	int reset();
	int setup();
	int setup_colors();
private:
	int gridSize_x = 50, gridSize_y = 50;

	Window* window = nullptr;
	Grid* mainGrid = nullptr;
	player* playerSnake = nullptr;
	Snake* collisionSNAKE = nullptr;
	client* serverConnection;

	int playerExpectedLength = 11;
	const int startLength = 3;
};

