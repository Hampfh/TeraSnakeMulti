#include "player.h"

player::player(Grid* mainGrid, int start_x, int start_y, int gridSize_x, int gridSize_y) {
	_mainGrid = mainGrid;
	
	_snakeHead_x = start_x;
	_snakeHead_y = start_y;
	_gridSize_x = gridSize_x;
	_gridSize_y = gridSize_y;
}

player::~player() {

}

void player::pollEvent(SDL_Event &evnt) {
	switch (evnt.type) {
		// This event get triggered when a key is pressed
	case SDL_KEYDOWN:
		switch (evnt.key.keysym.sym) {
		case SDLK_UP:
			direction != DIRECTION_DOWN ? direction = DIRECTION_UP : direction;
			break;
		case SDLK_RIGHT:
			direction != DIRECTION_LEFT ? direction = DIRECTION_RIGHT : direction;
			break;
		case SDLK_DOWN:
			direction != DIRECTION_UP ? direction = DIRECTION_DOWN : direction;
			break;
		case SDLK_LEFT:
			direction != DIRECTION_RIGHT ? direction = DIRECTION_LEFT : direction;
			break;
		}
	default:
		break;
	}
}

bool player::Update(int* expectedLength, ExternalSnake* enemy_first) {

	if (!dead) {
		move(direction);
		addNewLastPart();
	}

	if (enemy_first != nullptr) {
		auto* current = enemy_first;

		while (current != nullptr) {
			if (current->Collision(_snakeHead_x, _snakeHead_y)) {
				dead = true;
				break;
			}

			current = current->next;
		}
	}
	// TODO snake should crash into itself

	if (snakeLength > *expectedLength) {
		removeFirstPart();
	}

	draw();

	return false;
}