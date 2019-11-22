#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Map.h"
#include "PlayerStrategies.h"
using namespace std;

// [IMPORTANT] Data members in this struct are not pointers because we thought when doing in A1 that we were allowed to 
// have non-pointer types in structs. We did not have time to fix this in A2 because we received our grade a bit late 
// to know that this was a problem.
// We will fix this in A3 if need be.
struct Card
{
	Card() {};
	Card(int id, enum Card_Type type);
	Card(const Card &c);
	Card& operator=(const Card &c);

	int countryId;
	enum Card_Type type;	
};

enum Card_Type
{
	INFANTRY,
	ARTILLERY,
	CAVALRY,
	WILD
};

class Deck
{
public:
	Deck(int totalCountries);	// Constructor. Creates and shuffles deck.
	~Deck();					// Destructor.
	Deck(const Deck &d);
	Deck& operator=(const Deck &d);

	//	Draws a card from top of deck. IMPORTANT: always use isEmpty on deck first to check.
	Card draw();
	//	Check if deck has no more cards
	bool isEmpty() { return cardDeck->empty(); };
	//	Get bonus army from successful exchange.
	int getExchangedArmies();

private:
	vector<Card>* cardDeck;	// Deck; container of cards
	int* exchangeArmies; // Number of bonus armies given after a successful exchange

	//	Creates unshuffled deck equal to total number of countries
	void createDeck(int totalCountries);
	//	Shuffle the deck
	void shuffleDeck();
};

class Hand
{
public:
	Hand(Deck* deck, map<int,Country*>* countries, PlayerStrategy* strat);
	~Hand();
	Hand(const Hand &h);
	Hand& operator=(const Hand &h);

	//  Get number of cards in player's hand.
	int getHandCount() { return playerHand->size(); };
	//	Set player strategy
	void setPlayerStrategy(PlayerStrategy* strat) { strategy = strat; };
	//  Draw a card from the deck and put it in the player's hand.
	void drawFromDeck();
	//	Exchange 3 cards in hand for armies.
	int exchange();
	//	Display cards in player's hand.
	void showHand();

private:
	vector<Card>* playerHand;
	Deck* deck;		// Determines correct number of armies to give for exchange.
	map<int,Country*>* ownedCountries;	// Needed to iterate +2 bonus armies on countries based on exchanged cards.
	PlayerStrategy* strategy;

	//	Returns a string version of enum Card_Type.
	string getEnumString(Card_Type type);
	//  Validate if the 3 selected cards are matchings or consecutives
	bool isValidExchangeCards(int i, int j, int k);

	friend class PlayerStrategy;
	friend class AggressivePlayerStrategy;
	friend class BenevolentPlayerStrategy;
	friend class RandomPlayerStrategy;
	friend class CheaterPlayerStrategy;
};