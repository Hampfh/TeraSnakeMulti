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

bool player::Update(int* expectedLength, ExternalSnake* enemy_first, ExternalSnake* enemy_last) {
	addCoordinates.clear();
	removeCoordinates.clear();

	if (!PLAYER_DEAD) {
		move(direction);
		addNewLastPart();
		std::vector<int> coordinate;
		coordinate.push_back(_lastNode->link->getCoords().x);
		coordinate.push_back(_lastNode->link->getCoords().y);
		addCoordinates.push_back(coordinate);
	}

	// TODO iterates through all players
	if (enemy_first != nullptr) {
		auto* current = enemy_first;

		while (current != nullptr) {
			if (current->Collision(_snakeHead_x, _snakeHead_y)) {
				PLAYER_DEAD = true;
				continue;
			}

			current = current->next;
		}
	}

	if (snakeLength > *expectedLength) {
		std::vector<int> coordinate;
		coordinate.push_back(_firstNode->link->getCoords().x);
		coordinate.push_back(_firstNode->link->getCoords().y);
		removeCoordinates.push_back(coordinate);
		removeFirstPart();
	}

	draw();

	return false;
}

std::string player::SnakeToString() {
	Node* currentNode = _firstNode;

	std::string messageOut = "";
	std::stringstream ss;

	PLAYER_DEAD == false ? messageOut.append("A<|") : messageOut.append("D<|");

	// Start with the nodes to add
	messageOut.append("a{|");
	for (const auto& coordinate : addCoordinates) {
		messageOut.append(std::to_string(coordinate[0]));
		messageOut.append(":");
		messageOut.append(std::to_string(coordinate[1]));
		messageOut.append("|");
	}
	messageOut.append("}");

	// Create segment of remove coordinates
	messageOut.append("r{|");
	for (const auto& coordinate : removeCoordinates) {
		messageOut.append(std::to_string(coordinate[0]));
		messageOut.append(":");
		messageOut.append(std::to_string(coordinate[1]));
		messageOut.append("|");
	}
	messageOut.append("}");
	messageOut.append(">");
	return messageOut;
}