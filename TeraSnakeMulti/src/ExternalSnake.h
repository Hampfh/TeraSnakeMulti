#pragma once
#include "Snake.h"
#include <vector>

class ExternalSnake : public Snake {
public:
	ExternalSnake(Grid* main_grid, int start_x, int start_y, int grid_width, int grid_height);
	~ExternalSnake();

	void Update(const int* length);

	void SetDirection(int dir);

	int id;
	ExternalSnake* next = nullptr;
};

