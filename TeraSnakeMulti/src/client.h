#pragma once
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <regex>
#include "definitions.h"
#include "Snake.h"
#pragma comment(lib, "ws2_32.lib")

class client
{
public:
	client(std::string ipAdress = "127.0.0.1", int port = 54000);
	~client();
	int sendMessage(std::string message);
	int recvMessage(std::string *message);
	void getCollision(Snake* collisions, bool *dead);
	bool recvdEcho();
	void disconnect();

	std::vector<std::vector<int>> StripCoordinates(std::string string) const;
	int GetId() const { return clientId_; };
private:
	int clientId_;
	SOCKET sock;
	int sendResult;

	// In and Output from server
	char messageIn[1024];
	std::string messageOut;
};

