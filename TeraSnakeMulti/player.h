#pragma once
#include <SDL.h>
#include <Grid.h>
#include "Snake.h"
#include <sstream>

class player : public Snake
{
public:
	player(Grid* mainGrid, int start_x, int start_y, int gridSize_x, int gridSize_y);
	~player();
	void pollEvent(SDL_Event &evnt);
	bool update(int* expectedLength, Snake* collisions);
	std::string SnakeToString();
public:
	bool PLAYER_DEAD = false;
};

