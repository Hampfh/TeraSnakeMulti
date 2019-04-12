#pragma once
#include "client.h"
#include <Grid.h>
#include "player.h"
#include "ExternalSnake.h"
#include <Window.h>
#include <ctime>
#include <time.h>
#include <chrono>

class Game
{
public:
	Game(Window* main_window);
	~Game();
	int Loop();
	int reset();
	int setup();
	int setup_colors();
	void Interpret(const std::string& incoming);
	ExternalSnake* GetSnake(int id) const;
	void RemoveExternalSnake(int id);
	ExternalSnake* AddExternalSnake(int id, int x, int y);
	void UpdateExternals() const;
private:
	int gridWidth = 50, gridHeight = 50;

	bool gameRunning_;

	Window* window = nullptr;
	Grid* mainGrid = nullptr;
	player* playerSnake = nullptr;
	
	client* serverConnection_;

	ExternalSnake* enemyFirst_ = nullptr;
	ExternalSnake* enemyLast_ = nullptr;

	int playerExpectedLength = 30;
	const int startLength = 3;

	bool running = true;
};

