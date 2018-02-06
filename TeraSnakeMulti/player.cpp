#include "player.h"

player::player(Grid* mainGrid, int start_x, int start_y) {
	_mainGrid = mainGrid;
	
	_snakeHead_x = start_x;
	_snakeHead_y = start_y;
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

bool player::update(int* expectedLength) {
	move(direction);
	addNewLastPart();

	if (snakeLength > *expectedLength) {
		removeFirstPart();
	}

	//draw();

	return false;
}

std::string player::SnakeToString(bool Add) {
	Node* currentNode = _firstNode;

	int tempX, tempY;
	std::string messageOut = "";
	std::stringstream ss;

	Add == true ? messageOut.append("A<|") : messageOut.append("R<|");
	
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