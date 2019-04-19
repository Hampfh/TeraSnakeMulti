#include "Player.h"

Player::Player(Grid* mainGrid, int start_x, int start_y, int gridSize_x, int gridSize_y) {
	this->mainGrid = mainGrid;
	
	_snakeHead_x = start_x;
	_snakeHead_y = start_y;
	_gridSize_x = gridSize_x;
	_gridSize_y = gridSize_y;
}

Player::~Player() {

}

void Player::PollEvent(SDL_Event &evnt) {
	switch (evnt.type) {
		// This event get triggered when a key is pressed
	case SDL_KEYDOWN:
		switch (evnt.key.keysym.sym) {
		case SDLK_UP:
			direction_ != DIRECTION_DOWN ? direction_ = DIRECTION_UP : direction_;
			break;
		case SDLK_RIGHT:
			direction_ != DIRECTION_LEFT ? direction_ = DIRECTION_RIGHT : direction_;
			break;
		case SDLK_DOWN:
			direction_ != DIRECTION_UP ? direction_ = DIRECTION_DOWN : direction_;
			break;
		case SDLK_LEFT:
			direction_ != DIRECTION_RIGHT ? direction_ = DIRECTION_LEFT : direction_;
			break;
		}
	default:
		break;
	}
}

bool Player::Update(int* expectedLength, ExternalSnake* enemy_first) {

	if (!dead) {
		move(direction_);
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

	if (length_ > *expectedLength) {
		removeFirstPart();
	}

	draw();

	return false;
}