#pragma once
#include <SDL.h>
#include <Grid.h>
#include "Snake.h"
#include "ExternalSnake.h"
#include <sstream>
#include <vector>

class Player : public Snake
{
public:
	Player(Grid* mainGrid, int start_x, int start_y, int gridSize_x, int gridSize_y);
	~Player();
	void PollEvent(SDL_Event &evnt);
	bool Update(int* expectedLength, ExternalSnake* enemy_first);
	std::string SnakeToString();

	bool dead = false;
};

