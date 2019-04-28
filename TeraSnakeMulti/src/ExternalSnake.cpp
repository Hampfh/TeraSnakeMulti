#include "ExternalSnake.h"

ExternalSnake::ExternalSnake(Grid* main_grid, const int start_x, const int start_y, const int grid_width, const int grid_height) {
	id = -1;
	mainGrid = main_grid;

	_snakeHead_x = start_x;
	_snakeHead_y = start_y;
	_gridSize_x = grid_width;
	_gridSize_y = grid_height;
}


ExternalSnake::~ExternalSnake() {
	auto current = _firstNode;
	auto prev = _firstNode;
	while (current != nullptr) {
		current = current->next;
		delete prev;
		prev = current;
	}
	_firstNode = nullptr;
	_lastNode = nullptr;
}

void ExternalSnake::Update(const int* const length) {
	move(direction_);
	addNewLastPart();

	if (length_ > * length) {
		removeFirstPart();
	}
}

void ExternalSnake::SetDirection(const int dir) {
	if (dir >= 0 && dir <= 3) {
		direction_ = dir;
	}
};
