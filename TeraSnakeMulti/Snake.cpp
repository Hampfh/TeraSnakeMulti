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

void Snake::addNewSpecificPart(int x, int y) {
	Node* temp = new Node();
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
	Node* currentPointer = _firstNode;

	if (_firstNode == nullptr && _lastNode == nullptr) {
		return false;
	}

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
		currentPointer->link->setColor(color.r, color.g, color.b);
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
		break;
	case DIRECTION_RIGHT:
		_snakeHead_x++;
		break;
	case DIRECTION_DOWN:
		_snakeHead_y++;
		break;
	case DIRECTION_LEFT:
		_snakeHead_x--;
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