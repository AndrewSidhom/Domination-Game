//#include <iostream>
//#include <list>
//#include <map>
//#include "Dice.h"
//#include "Cards.h"
//using namespace std;
//
///*
//	The CardsDriver tests the Cards classes. It generates Countries and Cards supposedly owned by the user.
//	It then...
//		1. Requests user input for exchanging cards (max number of cards before exchange threshold has been reached)
//		2. Requests user input for where to place armies obtained from the card exchange
//		3. Requests user input again for exchanging cards, but also allows to cancel card exchange
//*/
//
//int main() {
//
//	//Create deck and hand object
//	Deck d(14);
//	map<int, int> ownedCountries;
//	map<int, int> ownedCountries2;
//
//	//Make country data
//	ownedCountries[1] = 3;
//	ownedCountries[2] = 4;
//	ownedCountries[3] = 6;
//	ownedCountries[4] = 2;
//	ownedCountries[5] = 5;
//	ownedCountries[6] = 1;
//	ownedCountries[7] = 8;
//
//	ownedCountries2[8] = 3;
//	ownedCountries2[9] = 4;
//	ownedCountries2[10] = 6;
//	ownedCountries2[11] = 2;
//	ownedCountries2[12] = 5;
//	ownedCountries2[13] = 1;
//	ownedCountries2[14] = 8;
//	
//	Hand h1(&d, &ownedCountries);
//	Hand h2(&d, &ownedCountries2);
//
//	//Draw all cards from deck and show hand
//	while (!d.isEmpty()) {
//		h1.addCardToHand(d.draw());
//
//		if(!d.isEmpty() && h2.getHandCount() < 4)				// double check h1 didnt already take the last card from deck
//			h2.addCardToHand(d.draw());	// otherwise, theres none for h2 to draw
//	}
//
//	cout << "Hand 1:\n--------" << endl;
//	h1.showHand();
//	cout << "\nHand 2:\n--------" << endl;
//	h2.showHand();
//
//	cout << "\nShow distribution of types in deck:";
//	cout << "\nInfantry count: " << h1.armies[0] + h2.armies[0] << endl;
//	cout << "Artillery count: " << h1.armies[1] + h2.armies[1] << endl;
//	cout << "Cavalry count: " << h1.armies[2] + h2.armies[2] << endl;
//
//	cout << "\nTesting mandatory exchange on hand 1..." << endl;
//	cout << "Owned Countries Before Exchange:\n----------------" << endl;
//	for (int i = 1; i < 8; i++) {
//		cout << "Country " << i << ": " << ownedCountries[i] << " armies" << endl;
//	}
//	h1.exchange();
//
//	cout << "Owned Countries After Exchange:\n----------------" << endl;
//	for (int i = 1; i < 8; i++) {
//		cout << "Country " << i << ": " << ownedCountries[i] << " armies" << endl;
//	}
//
//	cout << "\nTesting optional exchange on hand 2..." << endl;
//	cout << "Owned Countries Before Exchange:\n----------------" << endl;
//	for (int i = 8; i < 15; i++) {
//		cout << "Country " << i << ": " << ownedCountries2[i] << " armies" << endl;
//	}
//	h2.exchange();
//
//	cout << "Owned Countries After Exchange:\n----------------" << endl;
//	for (int i = 8; i < 15; i++) {
//		cout << "Country " << i << ": " << ownedCountries2[i] << " armies" << endl;
//	}
//
//	system("pause");
//	return 0;
//}