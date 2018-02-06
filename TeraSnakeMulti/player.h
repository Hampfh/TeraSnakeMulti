#pragma once
#include <SDL.h>
#include <Grid.h>
#include "Snake.h"
#include <sstream>

class player : public Snake
{
public:
	player(Grid* mainGrid, int start_x, int start_y);
	~player();
	void pollEvent(SDL_Event &evnt);
	bool update(int* expectedLength);
	std::string SnakeToString(bool Add);
};

