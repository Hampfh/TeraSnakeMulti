#include "Game.h"

void pollEvents(Window &window, player &player) {
	SDL_Event evnt;

	if (SDL_PollEvent(&evnt)) {
		window.pollEvent(evnt);
		player.pollEvent(evnt);
	}
}

std::string getIP() {
	std::string input;
	std::cout << "ENTER SERVER IP FOLLOWED BY THE PORT - EXAMPLE: 127.0.0.1:54000\n> ";
	std::getline(std::cin, input);
	return input;
}

Game::Game(Window* mainWindow) : window(mainWindow) {
	setup();

	serverConnection = new client();
}


Game::~Game()
{
}

int Game::loop() {

	Sleep(10);
	while (true) {
		collisionSNAKE = new Snake();
		collisionSNAKE->_mainGrid = mainGrid;
		collisionSNAKE->setColor(20, 250, 20);
		pollEvents(*window, *playerSnake);
		
		playerSnake->update(&playerExpectedLength);
		
		// Send nodes to 
		if (serverConnection->sendMessage(playerSnake->SnakeToString(true)) == 0) {
			delete window;
			std::cout << "STATUS> Connection lost" << std::endl;
			serverConnection->disconnect();
			return 1;
		}
		Sleep(1);
		serverConnection->getCollision(collisionSNAKE);

		collisionSNAKE->draw();

		window->refresh();
		mainGrid->clear();

		if (window->isClosed()) {
			return -1;
		}
		delete collisionSNAKE;
	}
	return 0;
}

int Game::reset() {
	delete mainGrid;
	delete playerSnake;

	playerExpectedLength = startLength;
	return 1;
}

int Game::setup() {

	mainGrid = new Grid(gridSize_x, gridSize_y);
	mainGrid->setSpacing(0);
	mainGrid->setDotSize(15);
	mainGrid->clear();
	window->connectGrid(mainGrid);

	playerSnake = new player(mainGrid, 20, 20);

	setup_colors();

	return 1;
}

int Game::setup_colors() {
	mainGrid->setColor(50, 50, 50);
	playerSnake->setColor(0, 180, 50);
	return 1;
}