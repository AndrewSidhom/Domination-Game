#include "Cards.h"
#include <cstdlib>
#include <iomanip>


/*	Constructor that creates and shuffles the deck
	@param total number of countries
*/
Cards::Cards(int totalCountries) {

	createDeck(totalCountries);
	shuffleDeck();

	// TODO Remove after testing
	createDummyCountries(totalCountries);
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

// TODO remove after testing
void Cards::createDummyCountries(int n) {

	for (int i = 0; i < n; i++) {
		DummyCountry c;
		c.id = i + 1;
		c.armies = (rand() % 99) + 1;
		ownedCountries->push_back(c);
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
	@return if exchange was successful, false if exchange cancelled
*/
bool Cards::Hand::exchange(vector<DummyCountry>* ownedCountries, bool isMandatory) {

	bool isSuccessful = false;
	int index = 0, cardsIndexToExchange[3];
	char playerInput;

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
			/// note vector->size() code blow this returns a size_t, NOT int
			/// hence why its declared size_t
			size_t selectedCardIndex = playerInput - '0'; /// implicitly cast char to int
			
			/// make sure index is within hand's card count
			if (selectedCardIndex <= playerHand->size()) 
			{
				cardsIndexToExchange[index] = playerInput - '0';
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
			if (isValidExchangeCards(cardsIndexToExchange[0], cardsIndexToExchange[1], cardsIndexToExchange[2]))
			{
				/// give bonus +2 armies if cards match owned countries
				giveBonusTwoArmies(ownedCountries, cardsIndexToExchange);
				/// remove exchanged cards from hand
				playerHand->erase(playerHand->begin() + cardsIndexToExchange[0]);
				playerHand->erase(playerHand->begin() + cardsIndexToExchange[1]);
				playerHand->erase(playerHand->begin() + cardsIndexToExchange[2]);
				/// exchange is successful
				isSuccessful = true;
			}
			else 
			{
				cout << "Cannot exchange with these cards. Must be a matching or of consecutive types.";
				index = 0;
			}
		}

	} while (index != 3);

	return isSuccessful;
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

/*	Check if exchanged cards match a country the player owns. If so,
	prompt the player to choose which country to give +2 units.
	@param owned countries
	@param index of cards in player's hand that will be exchanged
*/
void Cards::Hand::giveBonusTwoArmies(vector<DummyCountry>* ownedCountries, int cardsToExchange[]) {

	vector<DummyCountry> matchingCountries;
	char playerInput;

	/// check if exchanged cards matches owned countries
	for (int i = 0; i < 3; i++) {
		for (DummyCountry &c : *ownedCountries)
		{
			if (c.id == playerHand->at(cardsToExchange[i]).countryId)
				matchingCountries.push_back(c); /// store country by reference
		}
	}

	if (!matchingCountries.empty()) 
	{
		/// prompt player input to choose which country to give +2 units
		cout << "Countries you own matches with your exchanged cards:" << endl;
		for (DummyCountry c : matchingCountries) 
			cout << "Country " << c.id << ": " << c.armies << " armies" << endl;
		
		do {
			cout << "Choose a country to give 2 additional armies: ";
			cin >> playerInput;

			if (isdigit(playerInput))
			{
				int selectedCountry = playerInput - '0'; /// implicitly cast char to int

				for (DummyCountry &c : matchingCountries) /// grab country by reference
				{
					if (c.id == selectedCountry) {
						c.armies += 2;
						break;
					}
				}
				/// else if did not break, then choice was invalid
				cout << "Your choice is invalid. Please try again";
			}
			else
			{
				cout << "Invalid input. Please try again.";
			}
		} while (true);
	}
	
	
}