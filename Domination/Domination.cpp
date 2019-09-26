// Domination.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Cards.h"

int main()
{
	Cards c = Cards(8);
	Cards::Hand h;

	if(!c.isEmpty()) {
		h.addCardToHand(c.draw());
		h.addCardToHand(c.draw());
		h.addCardToHand(c.draw());
	}
	h.showHand();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


/*
	// TODO remove after testing
	for (Card c : *deck)
	{
		cout << "id: " << c.countryId << " type: ";
		switch (c.type)
		{
		case INFANTRY: cout << "Infantry"; break;
		case ARTILLERY: cout << "Artillery"; break;
		case CAVALRY: cout << "Cavalry"; break;
		case WILD: cout << "Wild";
		}
		cout << endl;
	}
*/
// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
