#include "Cards.h"
#include <cstdlib>
#include <iomanip>
#include <random>
#include <functional>

// Card struct constructor
Card::Card(int id, Card_Type type) : countryId(id), type(type){}
Card::Card(const Card &card) : countryId(card.countryId), type(card.type){} 
Card& Card::operator=(const Card &c) { 
	if(&c != this) {
		countryId = c.countryId; 
		type = c.type;
	}
	return *this; 
}

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

/*	Copy constructor of Cards class.
*/
Deck::Deck(const Deck &d) : cardDeck(d.cardDeck), exchangeArmies(d.exchangeArmies) {}

/*	Copy assignment operator.
*/
Deck& Deck::operator=(const Deck &d) 
{
	if(&d != this) {
		delete cardDeck, exchangeArmies;
		cardDeck = new vector<Card>();
		for (auto iter = d.cardDeck->begin(); iter != d.cardDeck->end(); ++iter) {
			Card c(*iter);
			cardDeck->push_back(c);
		}
		exchangeArmies = new int(*d.exchangeArmies);
	}
	return *this;
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
		default: break;
		}
		// assign country id from 1 and up
		c.countryId = ++n;
		// insert card to end of vector deck
		cardDeck->push_back(c);
	} while (n < totalCountries - 2); /// last 2 for wild cards

	/// add 2 wild cards to complete deck
	cardDeck->push_back(Card(n+1, WILD));
	cardDeck->push_back(Card(n+2, WILD));
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

/*	Constructor.
*/
Hand::Hand(Deck *deck, map<int,Country*> *countries, PlayerStrategy* aStrategy) : deck(deck), ownedCountries(countries), playerHand(new vector<Card>()), strategy(aStrategy) {}

/*	Destructor of Hand class.
*/
Hand::~Hand()
{
	delete playerHand;
	playerHand = nullptr;
}

/*	Copy constructor of Hand class.
*/
Hand::Hand(const Hand &h) {
	playerHand = new vector<Card>(*h.playerHand);
	deck = h.deck;
	ownedCountries = h.ownedCountries;
	strategy = h.strategy;
}

/*	Copy assignment operator of Hand class.
*/
Hand& Hand::operator=(const Hand &h) 
{
	if(&h != this) {
		delete playerHand;
		playerHand = new vector<Card>();
		for (auto iter = h.playerHand->begin(); iter != h.playerHand->end(); ++iter) {
			Card c(*iter);
			playerHand->push_back(c);
		}
		deck = h.deck;
		ownedCountries = h.ownedCountries;
		strategy = h.strategy;
	}
	return *this;
}

/*	Display cards in player's hand
*/
void Hand::showHand()
{
	cout << "\nYour hand:\n-------|\n";
	int i = 1;

	for (Card c : *playerHand)
	{
		cout << "CARD " << i << " | " << "country: " << c.countryId << ", type: " << getEnumString(c.type) << endl;
		i++;
	}
}

void Hand::drawFromDeck()
{
	if(!deck->isEmpty()) {
		Card c = deck->draw();
		playerHand->push_back(c);
	}
}

/*	Exchange 3 cards in hand for armies.
	@return exchanged armies, if exchange cancelled, return 0
*/
int Hand::exchange() 
{
	bool isMandatory = getHandCount() >= 5;

	if(!isMandatory && !strategy->ifPlayerWantsToExchange()) 
		return 0; 
	showHand();
	if (isMandatory)
		cout << "You have reached the maximum number of cards and must exchange. ";
	cout << "Enter 3 cards to exchange";
	if (!isMandatory)
		cout << " or enter 0 to cancel";
	cout << endl;

	return strategy->promptExchangeForArmies(isMandatory);
}

/*	Returns a string version of enum Card_Type
	@param Card type enum
*/
string Hand::getEnumString(Card_Type type)
{
	switch (type)
	{
	case INFANTRY:
		return "Infantry";
	case ARTILLERY:
		return "Artillery";
	case CAVALRY:
		return "Cavalry";
	case WILD:
		return "Wild";
	default:
		return "Card type not defined.";
	}
}

/*  Validate if the 3 selected cards are matchings or consecutives
	@param int i, j, k being the index of chosen card in hand
	@return true if valid, else false
*/
bool Hand::isValidExchangeCards(int i, int j, int k)
{
	return ( 
		/// if have at least one wild card
		(playerHand->at(i).type == WILD ||
		playerHand->at(j).type == WILD	||
		playerHand->at(k).type == WILD) ||
		/// OR if have 3 of one type
		(playerHand->at(i).type == playerHand->at(j).type &&
		 playerHand->at(i).type == playerHand->at(k).type) ||
		/// OR if have 1 of each type
		(playerHand->at(i).type != playerHand->at(j).type &&
		 playerHand->at(i).type != playerHand->at(k).type &&
		 playerHand->at(j).type != playerHand->at(k).type));
}