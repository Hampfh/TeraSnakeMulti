#include "Game.h"
#include <Window.h>
#include <Windows.h>
#include <iostream>

std::string playerName;
int main(int argc, char** argv) {
	Game game;
	const int state = game.Execute();

	switch (state) {
	case 1:
		system("pause");
		break;
	}
	return 0;
}