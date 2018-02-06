#pragma once
#include "definitions.h"
#include "Snake.h"
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <regex>
#pragma comment(lib, "ws2_32.lib")

class client
{
public:
	client(std::string ipAdress, int port);
	~client();
	int sendMessage(std::string message);
	int recvMessage(std::string *message);
	void getCollision(Snake* collisions);
	bool recvdEcho();
private:
	int CLIENT_ID;
	SOCKET sock;
	int sendResult;
	int bytesReceived;

	// In and Output from server
	char messageIn[4096];
	std::string messageOut;
};

