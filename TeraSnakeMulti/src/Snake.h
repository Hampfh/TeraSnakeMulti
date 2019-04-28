#pragma once
#include "Definitions.h"
#include <Grid.h>
class Snake {
public:
	Snake();
	~Snake();
	void addNewLastPart();
	void addNewSpecificPart(int x, int y, int r = -1, int g = -1, int b = -1);
	bool Collision(int x, int y);
	void Draw();
	void move(int direction);
	void move(int x, int y);
	void removeFirstPart();
	void removeSpecificPart(int x, int y);
	void setColor(int r, int g, int b);
	void SetHeadColor(int r, int g, int b);
	Node* getFirstNode() { return _firstNode; };
	Node* getLastNode() { return _lastNode; };
	colorTemplate getColor() { return color_; };
	int getDirection() { return direction_; };
	int GetHeadX() const { return _snakeHead_x; };
	int GetHeadY() const { return _snakeHead_y; };
protected:
	colorTemplate color_;
	colorTemplate head_;
	int length_;
	int direction_;

	int _snakeHead_x;
	int _snakeHead_y;

	int _gridSize_x, _gridSize_y;

	Node* _firstNode = nullptr;
	Node* _lastNode = nullptr;
public:
	int id;
	Grid* mainGrid = nullptr;
};

