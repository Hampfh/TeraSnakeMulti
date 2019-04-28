#pragma once
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <regex>
#include "definitions.h"
#include "Snake.h"
#pragma comment(lib, "ws2_32.lib")

class Client {
public:
	Client(std::string ip_address = "127.0.0.1", int port = 54000);
	~Client();
	int Send(std::string message);
	int Receive(std::string *message);
	std::vector<std::vector<int>> StripCoordinates(std::string string) const;

	void GetCollision(Snake* collisions, bool *dead);
	int GetId() const { return clientId_; };
private:
	int clientId_;
	SOCKET sock_;
	int sendResult_;

	// In and Output from server
	char messageIn_[1024];
	std::string messageOut_;
};

