#pragma once
#include "Client.h"
#include <Grid.h>
#include "Player.h"
#include "ExternalSnake.h"
#include <Window.h>
#include <ctime>
#include <chrono>

class Game {
public:
	Game();
	int Setup();
	int Execute();
	void PollEvents() const;
	void Loop();
	void Interpret(const std::string& incoming);
	ExternalSnake* GetSnake(int id) const;
	void RemoveExternalSnake(int id);
	void RemoveAllExternals();
	ExternalSnake* AddExternalSnake(int id, int x, int y);
	void UpdateExternals() const;
	void DrawExternals() const;
private:
	int gameTick_ = 0;

	int gridWidth_ = 50, gridHeight_ = 50;

	bool running_;
	bool gameRunning_;

	Window* window_ = nullptr;
	Grid* mainGrid_ = nullptr;
	Player* player_ = nullptr;
	
	Client* serverConnection_;

	ExternalSnake* enemyFirst_ = nullptr;
	ExternalSnake* enemyLast_ = nullptr;

	int playerExpectedLength_ = 30;

};

