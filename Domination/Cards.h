#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Map.h"
using namespace std;

struct Card
{
	enum Card_Type type;
	int countryId;
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

	//	Draws a card from top of deck. IMPORTANT: always use isEmpty on deck first to check.
	Card draw();
	//	Check if deck has no more cards
	bool isEmpty();
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
	Hand(Deck* deck, map<int,Country*>* countries);		// Constructor.
	~Hand();											// Destructor.

	//	Display cards in player's hand.
	void showHand();
	//  Get number of cards in player's hand.
	int getHandCount();
	//	Add card to hand.
	void addCardToHand(Card c);
	//	Exchange 3 cards in hand for armies.
	int exchange();

private:
	vector<Card>* playerHand;
	Deck* deck;		// Determines correct number of armies to give for exchange.
	map<int,Country*>* ownedCountries;	// Needed to iterate +2 bonus armies on countries based on exchanged cards.

	//	Returns a string version of enum Card_Type.
	string getEnumString(Card_Type type);
	//	Prompt if player wants to exchange.
	bool playerWantsToExchange();
	//	Prompt user to choose which card from their hand to exchange.
	int getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen, int cardsToExchange[]);
	//  Validate if the 3 selected cards are matchings or consecutives
	bool isValidExchangeCards(int i, int j, int k);
	//	Check if exchanged cards match a country the player owns. If so, 
	//	prompt the player to choose which country to give +2 units.
	void giveBonusTwoArmies(int cardsToExchange[]);
	//	Prompt user to choose which country that matches the exchanged cards to receive +2 bonus armies.
	int getMatchingCountryChoice(vector<int> matchingCountries);
};