#include "Game.h"

std::string GetIp() {
	std::string input;
	std::cout << "ENTER SERVER IP\n> ";
	std::getline(std::cin, input);
	return input;
}

int GetPort() {
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

Game::Game() {
	//serverConnection_ = new Client(GetIp(), GetPort());
	serverConnection_ = new Client("127.0.0.1", 15000);
	
	// Check if connection is established
	if (serverConnection_->Send("") == 0)
		running_ = true;
	else
		running_ = false;

	gameRunning_ = false;
}

int Game::Execute() {

	Setup();

	while (running_) {
		PollEvents();
		Loop();

		if (window_->isClosed()) 
			running_ = false;
	}
	system("pause");
	return 0;
}

void Game::PollEvents() const {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		window_->pollEvent(event);
		if (player_ != nullptr && !player_->dead) {
			player_->PollEvent(event);
		}
	}
}

void Game::Loop() {

	std::string commandStorage;

	serverConnection_->Receive(&commandStorage);
#ifdef _DEBUG
	std::cout << "Received: " << commandStorage << std::endl;
#endif

	Interpret(commandStorage);

	// Update enemies
	if (gameRunning_)
		UpdateExternals();

	// Update Player
	if (gameRunning_ && player_ != nullptr && !player_->dead) {
		player_->Update(&playerExpectedLength_, enemyFirst_);

		commandStorage = "M" + std::to_string(player_->getDirection());
	}
	else if (player_ != nullptr && !player_->dead) {
		commandStorage = "Px" + std::to_string(player_->GetHeadX()) + "|Py" + std::to_string(player_->GetHeadY());
	}
	else {
		commandStorage = "";
	}

	// Detect death
	if (player_ != nullptr && player_->dead) {
		std::cout << "I died" << std::endl;
		commandStorage.append("|D");
		delete player_;
		player_ = nullptr;
	}

	// Send
	if (serverConnection_->Send(commandStorage) != 0) {
		std::cout << "STATUS> Connection lost" << std::endl;
		delete serverConnection_;
		delete window_;
		running_ = false;
		return;
	}

	window_->refresh(0, 0, 0);
	mainGrid_->clear();
}

int Game::Setup() {
	window_ = new Window("Tera Snake", 750, 840);

	mainGrid_ = new Grid(gridWidth_, gridHeight_);
	mainGrid_->setSpacing(0);
	mainGrid_->setDotSize(15);
	window_->connectGrid(mainGrid_);

	player_ = new Player(mainGrid_, 2 * serverConnection_->GetId(), 20, gridWidth_, gridHeight_);

	// Setup colors
	mainGrid_->setColor(50, 50, 60);
	player_->setColor(0, 180, 50);
	player_->SetHeadColor(20, 20, 50);

	mainGrid_->clear();
	return 1;
}

void Game::Interpret(const std::string& incoming) {

	if (incoming.empty() || incoming.length() <= 1) {
		return;
	}
	std::string copy = incoming;

	const std::regex regexClient("[^{}]+");
	const std::regex regexCommand("[a-zA-Z0-9\\/\\(\\)&:;=<>\\*]+");
	std::smatch mainMatcher;
	std::smatch secondMatcher;

	while (std::regex_search(copy, mainMatcher, regexClient)) {
		for (auto clientString : mainMatcher) {

			std::string commandCopy = clientString.str();

			ExternalSnake* current = nullptr;
			int id = -1;

			// Special commands
			if (commandCopy == "*|D") {
				// Remove all external snakes
				std::cout << "Removing all externals" << std::endl;
				RemoveAllExternals();
				break;
			}

			// Parse Client command
			while (std::regex_search(commandCopy, secondMatcher, regexCommand)) {

				// Get the Client from string
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
					
					// Specific Moving direction_
					if (command.str()[0] == 'M' && current != nullptr) {
						std::string value = command.str().substr(1, command.str().size() - 1);
						// Set snake direction_
						current->SetDirection(std::stoi(value));
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
					// Check if Client has disconnected
					else if (command.str()[0] == 'D' && current != nullptr) {
						std::cout << "Received remove command on snake: " << current->id << std::endl;
						RemoveExternalSnake(current->id);
					} 
					// Start game loop
					else if (command.str()[0] == 'S') {
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

void Game::RemoveExternalSnake(const int id) {
	ExternalSnake* current = enemyFirst_;
	ExternalSnake* prev = enemyFirst_;

	while (current != nullptr) {
		if (current->id == id) {
			
			if (current == enemyFirst_ && enemyFirst_ == enemyLast_) {
				enemyFirst_ = nullptr;
				enemyLast_ = nullptr;
			}
			else if (current == enemyFirst_) {
				enemyFirst_ = enemyFirst_->next;
			} else if (current == enemyLast_) {
				enemyLast_ = prev;
				enemyLast_->next = nullptr;
			} else {
				prev->next = current->next;
			}
			// Delete the snake
			delete current;
			
			return;
		}
		prev = current;
		current = current->next;
	}
}

void Game::RemoveAllExternals() {
	ExternalSnake* current = enemyFirst_;
	ExternalSnake* prev = enemyFirst_;

	while (current != nullptr) {

		current = current->next;
		delete prev;
		prev = current;
	}
	enemyFirst_ = nullptr;
	enemyLast_ = nullptr;
}

ExternalSnake* Game::AddExternalSnake(const int id, const int x, const int y) {
	ExternalSnake* newSnake = new ExternalSnake(mainGrid_, x, y, gridWidth_, gridHeight_);

	// Randomize a color_ for the new snake
	srand(id * 10);
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

void Game::UpdateExternals() const {
	ExternalSnake* current = enemyFirst_;

	while (current != nullptr) {
		current->Update(&playerExpectedLength_);
		current = current->next;
	}
}