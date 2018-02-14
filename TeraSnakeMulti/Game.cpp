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
	std::cout << "ENTER SERVER IP\n> ";
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
	std::string messageOut;

	while (true) {
		pollEvents(*window, *playerSnake);
			
		playerSnake->update(&playerExpectedLength, collisionSNAKE);

		delete collisionSNAKE;
		collisionSNAKE = new Snake();
		collisionSNAKE->_mainGrid = mainGrid;
		collisionSNAKE->setColor(20, 250, 20);

		messageOut = playerSnake->SnakeToString();

		// Send snake position to server
		if (serverConnection->sendMessage(messageOut) == 0) {
			delete window;
			std::cout << "STATUS> Connection lost" << std::endl;
			serverConnection->disconnect();
			return 1;
		}
		serverConnection->getCollision(collisionSNAKE, &playerSnake->PLAYER_DEAD);

		collisionSNAKE->draw();

		window->refresh();
		mainGrid->clear();

		if (window->isClosed()) {
			return -1;
		}
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

	playerSnake = new player(mainGrid, 20, 20, gridSize_x, gridSize_y);

	setup_colors();

	return 1;
}

int Game::setup_colors() {
	mainGrid->setColor(50, 50, 60);
	playerSnake->setColor(0, 180, 50);
	return 1;
}