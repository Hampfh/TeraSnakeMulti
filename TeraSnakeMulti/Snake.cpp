#include "Snake.h"

Snake::Snake() {
	color.r = 0;
	color.g = 0;
	color.b = 0;
}


Snake::~Snake() {
	Node* currentNode = _firstNode;
	Node* prev = _firstNode;

	while (currentNode != _lastNode) {
		prev = currentNode;
		currentNode = currentNode->next;
		delete prev;
	}
	delete _lastNode;
}

void Snake::addNewLastPart() {
	Node* temp = new Node();
	temp->thisNodeColor.r = -1;
	temp->thisNodeColor.g = -1;
	temp->thisNodeColor.b = -1;
	if (_firstNode == nullptr) {
		_firstNode = temp;
		_lastNode = temp;
	}
	else {
		_lastNode->next = temp;
		_lastNode = _lastNode->next;
	}
	_lastNode->link = _mainGrid->dot(_snakeHead_x, _snakeHead_y);
	snakeLength++;
}

void Snake::addNewSpecificPart(int x, int y, int r, int g, int b) {
	Node* temp = new Node();
	temp->thisNodeColor.r = r;
	temp->thisNodeColor.g = g;
	temp->thisNodeColor.b = b;
	if (_firstNode == nullptr) {
		_firstNode = temp;
		_lastNode = temp;
	}
	else {
		_lastNode->next = temp;
		_lastNode = _lastNode->next;
	}
	_lastNode->link = _mainGrid->dot(x, y);
	snakeLength++;
}

bool Snake::collision(int x, int y) {

	if (_firstNode == nullptr || _lastNode == nullptr) {
		return false;
	}

	Node* currentPointer = _firstNode;

	while (currentPointer != _lastNode) {
		if (currentPointer->link->getCoords().x == x && currentPointer->link->getCoords().y == y) {
			return true;
		}
		currentPointer = currentPointer->next;
	}
	if (currentPointer == _lastNode) {
		if (currentPointer->link->getCoords().x == x && currentPointer->link->getCoords().y == y) {
			return true;
		}
	}

	return false;
}

void Snake::draw() {
	Node* currentPointer = _firstNode;

	while (currentPointer != _lastNode) {
		if (currentPointer->thisNodeColor.r == -1 || currentPointer->thisNodeColor.g == -1 || currentPointer->thisNodeColor.b == -1) {
			currentPointer->link->setColor(color.r, color.g, color.b);
		}
		else {
			std::cout << "SPECIAL COLOR APPLIED" << std::endl;
			currentPointer->link->setColor(currentPointer->thisNodeColor.r, currentPointer->thisNodeColor.g, currentPointer->thisNodeColor.b);
		}
		currentPointer = currentPointer->next;
	}
	
	if (currentPointer != nullptr) {
		_lastNode->link->setColor(color.r, color.g, color.b);
	}
}

void Snake::move(int direction) {
	switch (direction) {
	case DIRECTION_UP:
		_snakeHead_y--;
		if (_snakeHead_y < 0) {
			_snakeHead_y = _gridSize_y - 1;
		}	
		break;
	case DIRECTION_RIGHT:
		_snakeHead_x++;
		if (_snakeHead_x > _gridSize_x - 1) {
			_snakeHead_x = 0;
		}
		break;
	case DIRECTION_DOWN:
		_snakeHead_y++;
		if (_snakeHead_y > _gridSize_y - 1) {
			_snakeHead_y = 0;
		}
		break;
	case DIRECTION_LEFT:
		_snakeHead_x--;
		if (_snakeHead_x < 0) {
			_snakeHead_x = _gridSize_x - 1;
		}
		break;
	}
}

void Snake::removeFirstPart() {
	Node* prev = _firstNode;
	_firstNode = _firstNode->next;
	delete prev;
	snakeLength--;
}
void Snake::removeSpecificPart(int x, int y) {
	Node* currentPointer = _firstNode;
	Node* prev = _firstNode;

	while (currentPointer != _lastNode) {
		if (currentPointer->link->getCoords().x == x && currentPointer->link->getCoords().y == y) {
			// If the specified node is the first
			if (currentPointer == _firstNode) {
				currentPointer = currentPointer->next;
				delete prev;
				_firstNode = currentPointer;
			}
			else {
				prev->next = currentPointer->next;
				delete currentPointer;
			}
			return;
		}

		prev = currentPointer;
		currentPointer = currentPointer->next;
	}
	// If the specified node is the last
	if (currentPointer->link->getCoords().x == x && currentPointer->link->getCoords().y == y && currentPointer == _lastNode) {
		prev->next = nullptr;
		_lastNode = prev;
		delete currentPointer;
	}
}

void Snake::setColor(int r, int g, int b) {
	color.r = r;
	color.g = g;
	color.b = b;
}