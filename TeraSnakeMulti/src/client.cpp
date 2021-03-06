#include "Client.h"

Client::Client(const std::string ip_address, int port){

	// Initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		std::cerr << "Can't start winsock2, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ == INVALID_SOCKET) {
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in an int structure
	sockaddr_in hint = sockaddr_in();
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock_, reinterpret_cast<const sockaddr*>(&hint), sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError << std::endl;
		closesocket(sock_);
		WSACleanup();
		return;
	}

	// Client receives welcome message, id and the server seed
	std::string incoming;
	Receive(&incoming);

	const std::regex regexClient("[^\\|]+");
	std::smatch matcher;

	std::vector<std::string> segment;

	while (std::regex_search(incoming, matcher, regexClient)) {
		segment.push_back(matcher[0].str());
		incoming = matcher.suffix().str();
	}
	
	std::cout << segment[0] << std::endl;
	std::cout << "Assigned id: " << segment[1] << " by server" << std::endl;
	std::cout << "Server seed: " << segment[2] << std::endl;
	clientId_ = std::stoi(segment[1]);
}

Client::~Client(){
	closesocket(sock_);
	WSACleanup();
	std::cout << "STATUS> Client disconnected" << std::endl;
}

int Client::Send(const std::string message) {
	// Temporarly saves the message inside the class
	messageOut_ = message;

	if (send(sock_, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR) {
		std::cout << "ERROR: " << WSAGetLastError() << std::endl;
		return 1;
	}
	
	return 0;
}

int Client::Receive(std::string *message) {
	ZeroMemory(messageIn_, 1024);
	const int bytesReceived = recv(sock_, messageIn_, 1024, 0);

	if (bytesReceived < 0) {
		return 1;
	}

	*message = std::string(messageIn_, bytesReceived + 1);

	return 0;
}

void Client::GetCollision(Snake* collisions, bool *dead) {

	std::string receivedMessage;

	const std::clock_t begin = clock();
	// Receive from server
	Receive(&receivedMessage);
	const std::clock_t end = clock();

	const double elapsedTime = double(end - begin);

	// Start interpreting data
	std::vector<std::vector<int>> addList;
	std::vector<std::vector<int>> removeList;

	// Convert incoming to command
	const std::string command = receivedMessage;
	std::string copy = command;
	
	
	// Match "add" coordinates
	const std::regex regexAdd("a\\{\\|([^}]+)\\|\\}");
	// Match "remove" coordinates
	const std::regex regexRemove("r\\{\\|([^}]+)\\|\\}");

	std::smatch mainMatcher;

	// Check if character has died
	if (command[0] == 'D' && *dead) {
		*dead = true;
	}

	// Find the coordinates to append
	while (std::regex_search(copy, mainMatcher, regexAdd)) {
		for (auto addSegment : mainMatcher) {
			// Strip out coordinates
			addList = StripCoordinates(addSegment.str());
		}
		copy = mainMatcher.suffix().str();
	}

	copy = command;

	// Find the coordinates to remove
	while (std::regex_search(copy, mainMatcher, regexRemove)) {
		for (auto removeSegment : mainMatcher) {
			// Strip out coordinates
			removeList = StripCoordinates(removeSegment.str());
		}
		copy = mainMatcher.suffix().str();
	}

	// Apply "add" data to collision
	for (const auto& coordinate : addList) {
		collisions->addNewSpecificPart(
			std::stoi(
				std::to_string(
					coordinate[0]
				)
			), 
			std::stoi(
				std::to_string(
					coordinate[1]
				)
			), 255, 100, 55
		);
	}
	// Apply "remove" data to collision
	for (const auto& coordinate : removeList) {
		collisions->removeSpecificPart(
			std::stoi(
				std::to_string(
					coordinate[0]
				)
		),
			std::stoi(
				std::to_string(
					coordinate[1]
				)
			)
		);
	}
}

std::vector<std::vector<int>> Client::StripCoordinates(std::string string) const {
	std::vector<std::vector<int>> coordinates;
	std::smatch matcher;
	const std::regex mainRegex("\\d+:\\d+");

	// Get coordinates form string
	// Strip Player position
	while (std::regex_search(string, matcher, mainRegex)) {
		int x = 0, y = 0;

		for (auto segment : matcher) {
			x = std::stoi(segment.str().substr(0, segment.str().find(":")));
			y = std::stoi(segment.str().substr(segment.str().find(":") + 1));
		}

		std::vector<int> coordinate = { x, y };
		coordinates.push_back(coordinate);

		// Remove this coordinate from the remaining coordinates
		string = matcher.suffix().str();
	}
	return coordinates;
}