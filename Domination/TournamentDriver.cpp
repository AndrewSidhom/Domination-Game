#include "GameEngine.h"

#include <iostream>
using namespace std;

int main() {
	int choice;
	cout << "Welcome to Domination! Please type 1 for Single-game mode and 2 for Tournament mode" << endl;
	cin >> choice;
	while (!cin.good() || choice < 1 || choice > 2) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid input. Enter 1 or 2:" << endl;
		cin >> choice;
	}
	if (choice == 1) { //single-game mode, uses no-parameter constructor
		GameEngine ge;
		ge.startGameLoop();
	}
	else { //tournament mode
		Tournament t;
		t.setUpWithUserInput();
		t.playTournament();
	}

	return 0;
}