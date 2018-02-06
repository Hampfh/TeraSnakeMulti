#include "Game.h"
#include <Window.h>
#include <Windows.h>
#include <iostream>

Window* window = new Window("TeraSnake", 750, 840);
std::string playerName;
int main(int argc, char** argv) {
	Game game(window);
	game.loop();

	return 0;
}