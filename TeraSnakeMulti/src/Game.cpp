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
	//serverConnection = new client(getIP(), getPORT());
	serverConnection_ = new client("127.0.0.1", 15000);
	setup();
}

Game::~Game() {
}

int Game::Loop() {

	std::string messageOut;
	int laps = 0;

	gameRunning_ = false;

	while (true) {
		if (playerSnake->PLAYER_DEAD && laps < 4) {
			playerSnake->PLAYER_DEAD = false;
		}
		playerSnake->PLAYER_DEAD = false;
		pollEvents(*window, *playerSnake);

		std::string outgoing;

		if (gameRunning_) {
			playerSnake->Update(&playerExpectedLength, enemyFirst_, enemyLast_);

			outgoing = "M" + std::to_string(playerSnake->getDirection());
		} else {
			outgoing = "Px" + std::to_string(playerSnake->GetHeadX()) + "|Py" + std::to_string(playerSnake->GetHeadY());
		}

		// Send snake position to server
		if (serverConnection_->sendMessage(outgoing) != 0) {
			std::cout << "STATUS> Connection lost" << std::endl;
			serverConnection_->disconnect();
			delete window;
			return 1;
		}

		std::string incoming;
		auto start = std::chrono::steady_clock::now();
		serverConnection_->recvMessage(&incoming);
		auto end = std::chrono::steady_clock::now();
		std::cout << "Receive took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

		Interpret(incoming);

		UpdateExternals();

		window->refresh();
		mainGrid->clear();

		if (window->isClosed()) {
			break;
		}
		laps++;
	}
	while (true) {
		
	}
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

	playerSnake = new player(mainGrid, 2 * serverConnection_->GetId(), 20, gridSize_x, gridSize_y);

	setup_colors();

	return 1;
}

int Game::setup_colors() {
	mainGrid->setColor(50, 50, 60);
	playerSnake->setColor(0, 180, 50);
	playerSnake->SetHeadColor(20, 20, 50);
	return 1;
}

void Game::Interpret(const std::string& incoming) {

	if (incoming.empty() || incoming.length() <= 1) {
		return;
	}
	std::string copy = incoming;

	const std::regex regexClient("[^{}]+");
	const std::regex regexCommand("[a-zA-Z0-9\\/\\(\\)&:;=<>]+");
	std::smatch mainMatcher;
	std::smatch secondMatcher;

	while (std::regex_search(copy, mainMatcher, regexClient)) {
		for (auto clientString : mainMatcher) {

			std::string commandCopy = clientString.str();

			ExternalSnake* current = nullptr;
			int id = -1;

			// Parse client command
			while (std::regex_search(commandCopy, secondMatcher, regexCommand)) {

				// Get the client from string
				if (id != 0) {
					// Convert string to id
					id = std::stoi(secondMatcher[0].str());

					// Check if id is new and id isn't 0 which is a server message
					current = GetSnake(id);
					if (current == nullptr && id != 0) {
						// Create new snake
						std::cout << "New snake: " << id << std::endl;
						current = AddExternalSnake(id, 2 * id, 20);
					}
					id = 0;
				}

				int x = NULL, y = NULL;
				for (auto command : secondMatcher) {
					// Check what kind of command the clients has sent
					
					// Specific Moving direction
					if (command.str()[0] == 'M' && current != nullptr) {
						std::string value = command.str().substr(1, command.str().size() - 1);
						current->move(std::stoi(value));
						current->addNewLastPart();
					}
					// Position x
					else if (command.str()[0] == 'P' && command.str()[1] == 'x') {
						std::string value = command.str().substr(2, command.str().size() - 2);
						x = std::stoi(value);
					}
					// Position y
					else if (command.str()[0] == 'P' && command.str()[1] == 'y') {
						std::string value = command.str().substr(2, command.str().size() - 2);
						y = std::stoi(value);
					}
					// Check if client has disconnected
					else if (command.str()[0] == 'D' && current != nullptr) {
						RemoveExternalSnake(current->id);
					} 
					// Start game loop
					else if (command.str()[0] == 'R') {
						gameRunning_ = true;
					}

					// Check if both x and y is set
					if (x != NULL && y != NULL && current != nullptr) {
						current->addNewSpecificPart(x, y);
						current->move(x, y);
						// Reset coordinates
						x = NULL;
						y = NULL;
					}
				}
				commandCopy = secondMatcher.suffix().str();
			}
		}
		copy = mainMatcher.suffix().str();
	}
}

ExternalSnake* Game::GetSnake(const int id) const {
	ExternalSnake* current = enemyFirst_;
	while (current != nullptr) {
		if (current->id == id) {
			return current;
		}
		current = current->next;
	}
	
	return nullptr;
}

void Game::RemoveExternalSnake(const int id) const {
	ExternalSnake* current = enemyFirst_;
	ExternalSnake* prev = nullptr;

	while (current != nullptr) {
		if (current->id == id) {
			if (prev != nullptr) {
				prev->next = current->next;
			}
			delete current;
		}
		prev = current;
		current = current->next;
	}
}

ExternalSnake* Game::AddExternalSnake(const int id, const int x, const int y) {
	ExternalSnake* newSnake = new ExternalSnake(mainGrid, x, y, gridSize_x, gridSize_y);

	// Randomize a color for the new snake
	time_t temp = id;
	srand(id);
	const int colorR = rand() % 255;
	const int colorG = rand() % 255;
	const int colorB = rand() % 255;
	newSnake->setColor(colorR, colorG, colorB);
	newSnake->SetHeadColor(
		(colorR + 20 >= 255 ? colorR - 20 : colorR + 20),
		(colorG + 20 >= 255 ? colorG - 20 : colorG + 20),
		(colorB + 20 >= 255 ? colorB - 20 : colorB + 20)
	);

	newSnake->id = id;

	if (enemyFirst_ == nullptr) {
		enemyFirst_ = newSnake;
		enemyLast_ = newSnake;
	} else {
		enemyLast_->next = newSnake;
		enemyLast_ = enemyLast_->next;
	}
	return newSnake;
}

void Game::UpdateExternals() {
	ExternalSnake* current = enemyFirst_;

	while (current != nullptr) {
		current->Update(&playerExpectedLength);
		current = current->next;
	}
}