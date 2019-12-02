#include "GameEngine.h"

#include <iostream>
using namespace std;

/* Allows the user to play games of Risk in:
1. Single-game mode
2. Tournament mode (with only computer players)
 */


int main() {
	string choiceToLoop; //'y' or 'n' for whether the user wants to play again
	do {
		int choice;  //'1' for single-game or '2' for tournament
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
		cout << endl << "Would you like to play again? (y/n)" << endl;
		cin >> choiceToLoop;
		while (choiceToLoop.compare("y") != 0 && choiceToLoop.compare("n") != 0) {
			cout << "Invalid input. Enter y or n:" << endl;
			cin >> choiceToLoop;
		}
	} while (choiceToLoop.compare("y") == 0);

	return 0;
}