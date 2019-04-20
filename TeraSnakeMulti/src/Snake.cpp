#include "Snake.h"

Snake::Snake() {
	color_.r = 0;
	color_.g = 0;
	color_.b = 0;

	direction_ = 0;
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
	_lastNode->link = mainGrid->dot(_snakeHead_x, _snakeHead_y);
	length_++;
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
	_lastNode->link = mainGrid->dot(x, y);
	length_++;
}

bool Snake::Collision(const int x, const int y) {

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
			currentPointer->link->setColor(color_.r, color_.g, color_.b);
		}
		else {
			currentPointer->link->setColor(currentPointer->thisNodeColor.r, currentPointer->thisNodeColor.g, currentPointer->thisNodeColor.b);
		}
		currentPointer = currentPointer->next;
	}
	
	// Draw the enemy head
	if (_lastNode != nullptr) {
		_lastNode->link->setColor(head_.r, head_.g, head_.b);
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
	default:
		return;
	}
	// If direction_ is valid then assign the snakes direction_ to it
	this->direction_ = direction;
}

void Snake::move(const int x, const int y) {
	_snakeHead_x = x;
	_snakeHead_y = y;
}

void Snake::removeFirstPart() {
	Node* prev = _firstNode;
	_firstNode = _firstNode->next;
	delete prev;
	length_--;
}
void Snake::removeSpecificPart(int x, int y) {
	Node* currentPointer = _firstNode;
	Node* prev = _firstNode;

	if (currentPointer == nullptr) {
		return;
	}

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
	color_.r = r;
	color_.g = g;
	color_.b = b;
}

void Snake::SetHeadColor(const int r, const int g, const int b) {
	head_.r = r;
	head_.g = g;
	head_.b = b;
}