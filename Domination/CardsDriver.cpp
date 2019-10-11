#include <iostream>
#include <list>
#include <map>
#include "Dice.h"
#include "Cards.h"
using namespace std;

/*
	The CardsDriver tests the Cards classes. It generates Countries and Cards supposedly owned by the user.
	It then...
				1. Requests user input for exchanging cards (max number of cards before exchange threshold has been reached)
				2. Requests user input for where to place armies obtained from the card exchange
				3. Requests user input again for exchanging cards, but also allows to cancel card exchange
*/

int main() {

	//Create deck and hand object
	Deck d(10);
	Hand h1;
	map<int, int> ownedCountries;

	//Make country data
	ownedCountries[1]= 3;
	ownedCountries[2] = 4;
	ownedCountries[3] = 6;
	ownedCountries[4] = 2;
	ownedCountries[5] = 5;
	ownedCountries[6] = 1;
	ownedCountries[7] = 8;

	cout << "Owned Countries Before Exchange:\n----------------" << endl;
	for (int i = 1; i < 8; i++) {
		cout << "Country " << i << ": " << ownedCountries[i] << " armies" << endl;
	}

	//Draw all cards from deck and show hand
	while (!d.isEmpty()) {
		h1.addCardToHand(d.draw());
	}
	h1.showHand();

	//Show distribution
	cout << "\nInfantry count for h1: " << h1.armies[0] << endl;
	cout << "Artillery count for h1: " << h1.armies[1] << endl;
	cout << "Cavalry count for h1: " << h1.armies[2] << endl;

	cout << "\nTesting exchange..." << endl;
	h1.exchange(&ownedCountries, &d, true);
	
	cout << "Owned Countries After Exchange:\n----------------" << endl;
	for (int i = 1; i < 8; i++) {
		cout << "Country " << i << ": " << ownedCountries[i] << " armies" << endl;
	}
	h1.showHand();

	cout << "\nTest cancelling exchange..." << endl;
	h1.exchange(&ownedCountries, &d, false);

	system("pause");
	return 0;
}