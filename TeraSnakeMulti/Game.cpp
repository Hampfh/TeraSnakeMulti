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
	std::cout << "ENTER SERVER IP FOLLOWED BY THE PORT - EXAMPLE: 127.0.0.1\n> ";
	std::getline(std::cin, input);
	return input;
}

int getPORT() {
	int port;
	std::string input;
	while (true) {
		std::cout << "ENTER PORT> ";
		std::getline(std::cin, input);
		try {
			port = std::stoi(input);
			break;
		}
		catch(...){
			std::cout << "ERROR: input has to be a number" << std::endl;
		}
	}
	return port;
}

Game::Game(Window* mainWindow) : window(mainWindow) {
	setup();
	serverConnection = new client(getIP(), getPORT());
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
		else {
			std::cout << "STATUS> Message: " << playerSnake->SnakeToString(true) << std::endl;
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
	mainGrid->setColor(50, 50, 60);
	playerSnake->setColor(0, 180, 50);
	return 1;
}