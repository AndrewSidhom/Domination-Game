#include "Cards.h"
#include <cstdlib>
#include <iomanip>
#include <random>

/*	Constructor that creates and shuffles the deck
	@param total number of countries
*/
Deck::Deck(int totalCountries)
{

	cardDeck = new vector<Card>();
	createDeck(totalCountries);
	shuffleDeck();
	exchangeArmies = new int(0);
}

/*	Destructor of Cards class.
*/
Deck::~Deck()
{
	delete cardDeck, exchangeArmies;
}

/*	Creates unshuffled deck equal to total number of countries
	@param total number of countries
*/
void Deck::createDeck(int totalCountries)
{

	int n = 0;
	do
	{
		Card c;
		/// distribute card types for each card EVENLY
		switch (n % 3)
		{
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
	} while (n < totalCountries - 2); /// last 2 for wild cards

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
void Deck::shuffleDeck()
{

	/// riffle shuffle is in linear time, faster than knuth shuffle
	/// https://dev.to/s_awdesh/everyday-im-shuffling-im-a-card--fire-1f8b

	int deckSize = cardDeck->size();
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, deckSize - 1);

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
Card Deck::draw()
{

	Card last = cardDeck->back();
	cardDeck->pop_back();

	return last;
}

/*	Check if deck has no more cards
	@return deck is empty boolean
 */
bool Deck::isEmpty()
{

	return cardDeck->empty();
}

/*	Get bonus army from successful exchange.
*/
int Deck::getExchangedArmies()
{

	int n = *exchangeArmies;

	if (n == 0) // first bonus
		*exchangeArmies += 4;
	else if (n >= 4 && n <= 10) // between 4 and 10, +2
		*exchangeArmies += 2;
	else if (n >= 12 && n < 15) // at 12, +3
		*exchangeArmies += 3;
	else if (n >= 15) // at 15 or above, +5
		*exchangeArmies += 5;

	return *exchangeArmies;
}

//*** IMPLEMENTATION FOR HAND ***//
Hand::Hand()
{
	playerHand = new vector<Card>();

	//REMOVE AFTER TESTING
	armies = new int[3];
	for (int i = 0; i < 3; i++)
		armies[i] = 0;
}
/*	Destructor of Hand class.
*/
Hand::~Hand()
{
	delete playerHand;

	//REMOVE AFTER TESTING
	delete[] armies;
}

/*	Display cards in player's hand
*/
void Hand::showHand()
{

	cout << "Your hand:" << endl
		 << "---------/" << endl;
	int i = 1;

	for (Card c : *playerHand)
	{
		cout << "CARD " << i << " | "
			 << "country: " << c.countryId << ", type: " << getEnumString(c.type) << endl;
		i++;
	}
}

/*	Add card to hand.
	@param Card object
*/
void Hand::addCardToHand(Card c)
{

	playerHand->push_back(c);
}

/*	Exchange 3 cards in hand for armies
	@param ownedCountries list (ref) to check matching country to give +2 armies
	@param deck object keeps track of how many armies to give
	@param isMandatory if exchange is mandatory if player has too many cards
	@return exchanged armies, if exchange cancelled, return 0
*/
int Hand::exchange(map<int, int> *ownedCountries, Deck *deck, bool isMandatory)
{

	int exchangedArmies = 0;
	int numOfCardsChosen = 0;
	int cardsToExchangeIndex[3];

	showHand();

	if (isMandatory)
		cout << "You have reached the maximum number of cards and must exchange. ";
	cout << "Enter 3 cards to exchange";
	if (!isMandatory)
		cout << " or enter 0 to cancel";
	cout << endl;

	do {
		/// get player input
		int selectedCardIndex = getPlayersCardOfChoice(isMandatory, numOfCardsChosen);
		/// process valid player input
		if (selectedCardIndex == 0 && !isMandatory)
		{
			cout << endl
				 << "Exchange action cancelled.";
			break;
		}
		else
		{
			cardsToExchangeIndex[numOfCardsChosen] = selectedCardIndex - 1;
			numOfCardsChosen++;
		}

		if (numOfCardsChosen == 3) {
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
			else {
				cout << "\nCannot exchange with these cards. Must be a matching or of consecutive types.";
				numOfCardsChosen = 0;
			}
		}
	} while (numOfCardsChosen != 3);

	return exchangedArmies;
}

/*	Returns a string version of enum Card_Type
	@param Card type enum
*/
string Hand::getEnumString(Card_Type type)
{

	switch (type)
	{
	case INFANTRY:
		armies[0] += 1;
		return "Infantry";
	case ARTILLERY:
		armies[1] += 1;
		return "Artillery";
	case CAVALRY:
		armies[2] += 1;
		return "Cavalry";
	case WILD:
		return "Wild";
	}
}

int Hand::getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen) {

	int selectedCardIndex;
	bool validInput = false;
	do
	{
		cout << "Card " << (numOfCardsChosen + 1) << ": ";

		cin >> selectedCardIndex;
		if (!cin.good())	/// !good() when input isnt integer
		{
			cout << "\nInvalid number input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else if (selectedCardIndex == 0 && isMandatory)
			cout << "\nYou've reached the card limit and must exchange.\n";
		else if (selectedCardIndex < 0 || selectedCardIndex > playerHand->size())
			cout << "\nYour choice must be within your hand's cards.\n";
		else
			validInput = true;
	} 
	while (!validInput);

	return selectedCardIndex;
}

/*  Validate if the 3 selected cards are matchings or consecutives
	@param int i, j, k being the index of chosen card in hand
	@return true if valid, else false
*/
bool Hand::isValidExchangeCards(int i, int j, int k)
{
	return ( /// if matching
		(playerHand->at(i).type == playerHand->at(j).type &&
		 playerHand->at(i).type == playerHand->at(k).type) ||
		/// OR if consecutive
		(playerHand->at(i).type != playerHand->at(j).type &&
		 playerHand->at(i).type != playerHand->at(k).type &&
		 playerHand->at(j).type != playerHand->at(k).type));
}

/*	Check if exchanged cards match a country the player owns. If so,
	prompt the player to choose which country to give +2 units.
	@param owned countries
	@param index of cards in player's hand that will be exchanged
*/
void Hand::giveBonusTwoArmies(map<int, int> *ownedCountries, int cardsToExchange[])
{
	vector<int> matchingCountries;
	char playerInput;

	/// check if exchanged cards matches owned countries
	for (int i = 0; i < 3; i++) {
		for (map<int, int>::iterator iter = ownedCountries->begin(); iter != ownedCountries->end(); ++iter)
		{
			if (playerHand->at(cardsToExchange[i]).countryId == iter->first)
				matchingCountries.push_back(iter->first);						  /// store country by reference
		}
	}

	if (!matchingCountries.empty())
	{
		/// prompt player input to choose which country to give +2 units
		cout << "\nCountries you own matches with your exchanged cards:\n";
		for (int id : matchingCountries)
			cout << "Country " << id << "|" << endl;

		(*ownedCountries)[getMatchingCountryChoice(matchingCountries)] += 2;
	}
}

/*	Prompt user to choose which country that matches the exchanged cards 
	to receive +2 bonus armies.
	@param country ids that matches exchanged cards
*/
int Hand::getMatchingCountryChoice(vector<int> matchingCountries) {

	int selectedCountryId;
	bool validInput = false;
	do
	{
		cout << "Choose a country to give 2 additional armies: ";
		cin >> selectedCountryId;
		
		if (!cin.good()) /// !good() when input isnt integer
		{
			cout << "\nInvalid number input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else {
			for(int id : matchingCountries) {
				if(selectedCountryId == id)
					validInput = true;
			}
			if(!validInput)
				cout << "\nPlease choose the countries given.\n";
		}
	} 
	while (!validInput);

	return selectedCountryId;
}