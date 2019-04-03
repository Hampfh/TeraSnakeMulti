#include "Game.h"
#include <Window.h>
#include <Windows.h>
#include <iostream>

Window* window = new Window("TeraSnake", 750, 840);
std::string playerName;
int main(int argc, char** argv) {
	Game game(window);
	int state = game.Loop();

	switch (state) {
	case 1:
		system("pause");
		break;
	}
	return 0;
}