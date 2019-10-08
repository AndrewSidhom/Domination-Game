#include "Cards.h"
#include <cstdlib>
#include <iomanip>
#include <random>


/*	Constructor that creates and shuffles the deck
	@param total number of countries
*/
Deck::Deck(int totalCountries) {
	
	cardDeck = new vector<Card>();
	createDeck(totalCountries);
	shuffleDeck();
	*exchangeArmies = 0;
}

/*	Destructor of Cards class.
*/
Deck::~Deck() {
	delete cardDeck;
}

/*	Creates unshuffled deck equal to total number of countries
	@param total number of countries
*/
void Deck::createDeck(int totalCountries) {

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
		cardDeck->push_back(c);
	} 
	while (n < totalCountries - 2); /// last 2 for wild cards

	/// add 2 wild cards to complete deck
	Card w1, w2;
	w1.type = WILD;
	w2.type = WILD;
	w1.countryId = n + 1;
	w2.countryId = n + 2;

	cardDeck->push_back(w1);
	cardDeck->push_back(w2);
};

/*	Shuffle the deck
*/
void Deck::shuffleDeck() {

	/// riffle shuffle is in linear time, faster than knuth shuffle
	/// https://dev.to/s_awdesh/everyday-im-shuffling-im-a-card--fire-1f8b

	int deckSize = cardDeck->size();
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, deckSize-1);

	for (int i = 0; i < deckSize; i++)
	{
		/// generate random num between 0 to deck size
		int rnd = dist(mt);
		/// each card will swap ONCE with another random card
		Card temp = cardDeck->at(i);
		cardDeck->at(i) = cardDeck->at(rnd);
		cardDeck->at(rnd) = temp;
	}
}

/*	Draws a card from top of deck. IMPORTANT: always use isEmpty on deck first to check.
	@return Card object
*/
Card Deck::draw() {

	Card last = cardDeck->back();
	cardDeck->pop_back();

	return last;
}

/*	Check if deck has no more cards
	@return deck is empty boolean	
 */ 
bool Deck::isEmpty() {

	return cardDeck->empty();
}

/*	Get bonus army from successful exchange.
*/
int Deck::getExchangedArmies() {

	int n = *exchangeArmies;

	if (n == 0)	// first bonus
		*exchangeArmies += 4;
	else if (n >= 4 && n <= 10) // between 4 and 10, +2
		*exchangeArmies += 2;
	else if (n >= 12 && n < 15) // at 12, +3
		*exchangeArmies += 3;
	else if (n >= 15)			// at 15 or above, +5
		*exchangeArmies += 5;

	return *exchangeArmies;
}

//*** IMPLEMENTATION FOR HAND ***//
Hand::Hand() {
	playerHand = new vector<Card>();
	
	//REMOVE AFTER TESTING
	armies = new int[3];
	for (int i=0;i<3;i++)
		armies[i] = 0;
}
/*	Destructor of Hand class.
*/
Hand::~Hand() {
	delete playerHand;

	//REMOVE AFTER TESTING
	delete armies; 
}

/*	Display cards in player's hand
*/
void Hand::showHand() {

	cout << "Your hand:" << endl << "---------/" << endl;
	int i = 1;

	for (Card c : *playerHand) {
		cout << "CARD " << i << " | "<< 
				"country: " << c.countryId << 
				", type: " << getEnumString(c.type) << endl;
		i++;
	}
}

/*	Add card to hand.
	@param Card object
*/
void Hand::addCardToHand(Card c) {

	playerHand->push_back(c);
}

/*	Exchange 3 cards in hand for armies
    @param ownedCountries[] reference to list of owned countries
	@param isMandatory if exchange is mandatory
	@return exchanged armies, if false, return 0
*/
int Hand::exchange(map<int,int> *ownedCountries, Deck *deck, bool isMandatory) {

	int exchangedArmies = 0;
	int index = 0, cardsToExchangeIndex[3];
	char playerInput;

	/// display hand to player
	showHand();
	/// prompt player input
	if (isMandatory) cout << "You have reached the maximum number of cards and must exchange. ";
	cout << "Enter 3 cards to exchange";
	if (!isMandatory) cout << " or 'x' to cancel";
	cout << endl;

	do {
		cout << endl << "Card " << (index + 1) << ": ";
		/// get player input with max length of 1
		cin >> setw(1) >> playerInput;

		if (!isMandatory && playerInput == 'x')
		{
			cout << endl << "Exchange action cancelled.";
			break;
		}
		else if (isdigit(playerInput))
		{
			/// note vector->size() code below this returns a size_t, NOT int
			/// hence why its declared size_t
			size_t selectedCardIndex = playerInput - '0'; /// implicitly cast char to int
			
			/// make sure index is within hand's card count
			if (selectedCardIndex <= playerHand->size() && selectedCardIndex > 0) 
			{
				cardsToExchangeIndex[index] = playerInput - '0';
				index++;
			}
			else cout << endl << "Your choice must be within your hand's cards.";
		}
		else
		{
			cout << endl << "Invalid input. Please try again.";
		}

		if (index == 3) /// if 3 cards are picked
		{
			if (isValidExchangeCards(cardsToExchangeIndex[0], cardsToExchangeIndex[1], cardsToExchangeIndex[2]))
			{
				/// give bonus +2 armies if cards match owned countries
				giveBonusTwoArmies(ownedCountries, cardsToExchangeIndex);
				/// remove exchanged cards from hand
				playerHand->erase(playerHand->begin() + cardsToExchangeIndex[0]);
				playerHand->erase(playerHand->begin() + cardsToExchangeIndex[1]);
				playerHand->erase(playerHand->begin() + cardsToExchangeIndex[2]);
				/// exchange is successful
				exchangedArmies = deck->getExchangedArmies();
			}
			else 
			{
				cout << endl << "Cannot exchange with these cards. Must be a matching or of consecutive types.";
				index = 0;
			}
		}

	} while (index != 3);

	return exchangedArmies;
}

/*	Returns a string version of enum Card_Type
	@param Card type enum
*/
string Hand::getEnumString(Card_Type type) {

	switch (type)
	{
	case INFANTRY: armies[0] += 1; return "Infantry";
	case ARTILLERY: armies[1] += 1; return "Artillery";
	case CAVALRY: armies[2] += 1; return "Cavalry";
	case WILD: return "Wild";
	}
}

/*  Validate if the 3 selected cards are matchings or consecutives
	@param int i, j, k being the index of chosen card in hand
	@return true if valid, else false
*/
bool Hand::isValidExchangeCards(int i, int j, int k) {

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
void Hand::giveBonusTwoArmies(map<int,int>* ownedCountries, int cardsToExchange[]) {

	vector<int> matchingCountries;
	char playerInput;

	/// check if exchanged cards matches owned countries
	for (int i = 0; i < 3; i++) {
		for (map<int,int>::iterator iter = ownedCountries->begin(); iter != ownedCountries->end(); ++iter)
		{
			if (iter->first == playerHand->at(cardsToExchange[i]).countryId)
				matchingCountries.push_back(iter->first); /// store country by reference
		}
	}

	if (!matchingCountries.empty()) 
	{
		/// prompt player input to choose which country to give +2 units
		cout << "\nCountries you own matches with your exchanged cards:\n";
		for (int id : matchingCountries) 
			cout << "Country " << id << "|" << endl;
		
		do {
			cout << "Choose a country to give 2 additional armies: ";
			cin >> playerInput;

			if (isdigit(playerInput))
			{
				int selectedCountry = playerInput - '0'; /// implicitly cast char to int
				for (int id : matchingCountries) /// grab country by reference
				{
					if (id == selectedCountry) {
						(*ownedCountries)[id] += 2;
						break;
					}
				}
				/// else if did not break, then choice was invalid
				cout << endl << "Your choice is invalid. Please try again" << endl;
			}
			else cout << endl << "Invalid input. Please try again." << endl;
			
		} while (true);
	}
	
	
}