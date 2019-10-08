#include <iostream>
#include <list>
#include "Dice.h"
#include "Cards.h"
using namespace std;

int main() {

	Deck d(60);
	Hand h1;

	while (!d.isEmpty()) {
		h1.addCardToHand(d.draw());
	}
	h1.showHand();

	cout << "\nInfantry count for h1: " << h1.armies[0] << endl;
	cout << "Artillery count for h1: " << h1.armies[1] << endl;
	cout << "Cavalry count for h1: " << h1.armies[2] << endl;

	//TODO - Figure out how exchange works (ownedCountries???)
	h1.exchange();

	system("pause");
	return 0;
}