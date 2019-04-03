#pragma once
#include <SDL.h>
#include <Grid.h>
#include "Snake.h"
#include "ExternalSnake.h"
#include <sstream>
#include <vector>

class player : public Snake
{
public:
	player(Grid* mainGrid, int start_x, int start_y, int gridSize_x, int gridSize_y);
	~player();
	void pollEvent(SDL_Event &evnt);
	bool Update(int* expectedLength, ExternalSnake* enemy_first, ExternalSnake* enemy_last);
	std::string SnakeToString();

	bool dead = false;
};

