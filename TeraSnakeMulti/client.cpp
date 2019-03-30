#include "client.h"

client::client(std::string ipAdress, int port){

	// Initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		std::cerr << "Can't start winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in an int structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::string temp;
	recvMessage(&temp);
	std::cout << "SERVER> " << temp << std::endl;
	recvMessage(&temp);
	std::cout << "SERVER> Your game ID is: " << temp << std::endl;
}

client::~client(){
	closesocket(sock);
	WSACleanup();
	std::cout << "STATUS> Client disconnected" << std::endl;
}

void client::disconnect() {
	delete this;
}

int client::sendMessage(std::string message) {
	// Temporarly saves the message inside the class
	messageOut = message;

	// Send message
	std::clock_t beginCon = clock();

	if (send(sock, message.c_str(), message.size() + 1, 0) != SOCKET_ERROR) {
		std::clock_t endCon = clock();
		//std::cout << endCon - beginCon << " milliseconds" << std::endl;
		return 1;
	}
	return 0;
}

int client::recvMessage(std::string *message) {
	ZeroMemory(messageIn, 1024);
	int bytesReceived = recv(sock, messageIn, 1024, 0);
	
	*message = messageIn;

	if (bytesReceived > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

void client::getCollision(Snake* collisions, bool *dead) {
	// Initialization
	int coord_x, coord_y;
	std::string receivedMessage;
	std::string copy = receivedMessage;
	std::regex getCoords("\\d+:\\d+");
	std::smatch main_matcher;
	std::regex getSnake("<(\\w)[^ >]+>");
	std::smatch second_matcher;

	std::clock_t begin = clock();
	// Receive from server
	recvMessage(&receivedMessage);
	std::clock_t end = clock();



	double elapsed_time = double(end - begin);
	//std::cout << elapsed_time << std::endl;
	receivedMessage[0] == 'A' ? *dead = false : *dead = true;
	
	// Use regex to get coordinates
	while (std::regex_search(receivedMessage, main_matcher, getCoords)) {
		for (auto x : main_matcher) {
			coord_x = std::stoi(x.str().substr(0, x.str().find(":")));
			coord_y = std::stoi(x.str().substr(x.str().find(":") + 1));
		}

		// Append values to the collisions object
		collisions->addNewSpecificPart(coord_x, coord_y);

		receivedMessage = main_matcher.suffix().str();
	}
}