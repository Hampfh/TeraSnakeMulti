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
	void getCollision(Snake* collisions, bool *alive);
	bool recvdEcho();
	void disconnect();
private:
	int CLIENT_ID;
	SOCKET sock;
	int sendResult;
	int bytesReceived;

	// In and Output from server
	char messageIn[4096];
	std::string messageOut;
};

