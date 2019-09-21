#include "Cards.h"
#include <cstdlib>
#include <iomanip>
using namespace std;


/*	Constructor that creates and shuffles the deck
	@param total number of countries
*/
Cards::Cards(int totalCountries) {

	createDeck(totalCountries);
	shuffleDeck();
}

/*	Creates unshuffled deck equal to total number of countries
	@param total number of countries
*/
void Cards::createDeck(int totalCountries) {

	int n = 0;

	do 
	{ 
		Card c;
		/// distribute card types for each card EVENLY
		switch (n % 3) {
		case 0:
			c.type = INFANTRY;
			break;
		case 1:
			c.type = ARTILLERY;
			break;
		case 2:
			c.type = CAVALRY;
			break;
		}
		/// assign country id starting from 1 and up
		c.countryId = ++n;
		/// insert new card to end of vector
		deck->push_back(c);
	} 
	while (n < totalCountries - 2); /// last 2 for wild cards

	/// add 2 wild cards to complete deck
	Card w1, w2;
	w1.type = WILD;
	w2.type = WILD;
	w1.countryId = n + 1;
	w2.countryId = n + 2;

	deck->push_back(w1);
	deck->push_back(w2);

	// TODO remove after testing
	for (Card c : *deck)
	{
		cout << "id: " << c.countryId << " type: ";
		switch (c.type)
		{
		case INFANTRY: cout << "Infantry";
		case ARTILLERY: cout << "Artillery";
		case CAVALRY: cout << "Cavalry";
		case WILD: cout << "Wild";
		}
	}
};

/*	Shuffle the deck
*/
void Cards::shuffleDeck() {

	/// riffle shuffle is in linear time, faster than knuth shuffle
	/// https://dev.to/s_awdesh/everyday-im-shuffling-im-a-card--fire-1f8b

	int deckSize = deck->size();

	for (int i = 0; i < deckSize; i++)
	{
		/// generate random num between 0 to deck size
		int rnd = rand() % deckSize;
		/// each card will swap ONCE with another random card
		Card temp = deck->at(i);
		deck->at(i) = deck->at(rnd);
		deck->at(rnd) = temp;
	}
}

/*	Draws a card from top of deck
	@return Card object
*/
Cards::Card Cards::draw() {

	Card last = deck->back();
	deck->pop_back(); /// remove last element
		
	return last;
}

//*** IMPLEMENTATION FOR HAND ***//

/*	Display cards in player's hand
*/
void Cards::Hand::showHand() {

	cout << "Your hand:" << endl << "--------------" << endl;
	for (Card c : *playerHand) {
		cout << "C1: country: " << c.countryId << ", type: " << getEnumString(c.type);
	}
}

/*	Exchange 3 cards in hand for armies
    @param ownedCountries[] reference to list of owned countries
	@param isMandatory if exchange is mandatory
*/
void Cards::Hand::exchange(int ownedCountries[], bool isMandatory) {

	int countryId, index = 0;
	char playerInput;
	int cardsToExchange[3];

	/// display hand to player
	showHand();
	/// prompt player input
	cout << "Enter 3 cards to exchange";
	if (!isMandatory) cout << " or 'x' to cancel";
	cout << endl;

	do {
		cout << "Card " << (index + 1) << ": ";
		/// get player input with max length of 1
		cin >> setw(1) >> playerInput;

		if (!isMandatory && playerInput == 'x')
		{
			cout << endl << "Exchange action cancelled.";
			break;
		}
		else if (isdigit(playerInput))
		{
			int cardInHandIndex = playerInput - '0'; /// implicitly cast char to int

			/// make sure index is within hand's card count
			if (cardInHandIndex <= playerHand->size())
			{
				cardsToExchange[index] = playerInput - '0';
				index++;
			}
			else cout << "Your choice must be within your hand's cards.";
		}
		else
		{
			cout << "Invalid input. Please try again.";
		}

		if (index == 3) /// if 3 cards are picked
		{
			if (isValidExchangeCards(cardsToExchange[0], cardsToExchange[1], cardsToExchange[2]))
			{
				// exchange and add units
			}
			else cout << "Cannot exchange with these cards. Must be a matching or of consecutive types.";
		}

	} while (index != 3);
}

/*	Returns a string version of enum Card_Type
	@param Card type enum
*/
string Cards::Hand::getEnumString(Card_Type type) {

	switch (type)
	{
	case INFANTRY: return "Infantry";
	case ARTILLERY: return "Artillery";
	case CAVALRY: return "Cavalry";
	case WILD: return "Wild";
	}
}

/*  Validate if the 3 selected cards are matchings or consecutives
	@param int i, j, k being the index of chosen card in hand
	@return true if valid, else false
*/
bool Cards::Hand::isValidExchangeCards(int i, int j, int k) {

	return ( /// if matching
		(playerHand->at(i).type == playerHand->at(j).type &&
		playerHand->at(i).type == playerHand->at(k).type) ||
		/// OR if consecutive
		(playerHand->at(i).type != playerHand->at(j).type &&
		playerHand->at(i).type != playerHand->at(k).type &&
		playerHand->at(j).type != playerHand->at(k).type)
	);
}