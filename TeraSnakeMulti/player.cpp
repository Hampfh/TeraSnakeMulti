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

bool player::update(int* expectedLength, Snake* collisions) {
	if (!PLAYER_DEAD) {
		move(direction);
		addNewLastPart();
	}

	if (collisions != nullptr && 
		collisions->collision(_snakeHead_x, _snakeHead_y) &&
		_snakeHead_x == _lastNode->link->getCoords().x && _snakeHead_y == _lastNode->link->getCoords().y) {
		PLAYER_DEAD = true;
	}

	if (snakeLength > *expectedLength) {
		removeFirstPart();
	}

	/*draw();

	this->_lastNode->link->setColor(20, 20, 50);*/

	return false;
}

std::string player::SnakeToString() {
	Node* currentNode = _firstNode;

	int tempX, tempY;
	std::string messageOut = "";
	std::stringstream ss;

	PLAYER_DEAD == false ? messageOut.append("A<|") : messageOut.append("D<|");

	while (currentNode != nullptr) {
		
		tempX = currentNode->link->getCoords().x;
		tempY = currentNode->link->getCoords().y;

		ss << tempX << ":" << tempY << "|";

		messageOut.append(ss.str());

		ss.str("");
		ss.clear();

		currentNode = currentNode->next;
	}



	messageOut.append(">");
	return messageOut;
}