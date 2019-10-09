#include <iostream>
#include <list>
#include <map>
#include "Dice.h"
#include "Cards.h"
using namespace std;

int main() {

	Deck d(60);
	Hand h1;
	map<int, int> ownedCountries;
	
	ownedCountries[10]= 3;
	ownedCountries[20] = 4;
	ownedCountries[30] = 6;
	ownedCountries[40] = 2;
	ownedCountries[50] = 5;
	ownedCountries[6] = 1;
	ownedCountries[17] = 8;

	while (!d.isEmpty()) {
		h1.addCardToHand(d.draw());
	}
	h1.showHand();

	cout << "\nInfantry count for h1: " << h1.armies[0] << endl;
	cout << "Artillery count for h1: " << h1.armies[1] << endl;
	cout << "Cavalry count for h1: " << h1.armies[2] << endl;

	cout << "\nTesting exchange..." << endl;
	h1.exchange(&ownedCountries, &d, true);

	system("pause");
	return 0;
}