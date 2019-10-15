#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "Map.h"
using namespace std;

struct Card
{
	enum Card_Type type;
	int countryId;
};

class Deck
{
public:
	/*	Constructor that creates and shuffles the deck
		@param total number of countries
	*/
	Deck(int totalCountries);

	/*	Destructor of Deck class.
	*/
	~Deck();

	/*	Draws a card from top of deck. IMPORTANT: always use isEmpty on deck first to check.
		@return Card object
	*/
	Card draw();

	/*	Check if deck has no more cards
		@return deck is empty boolean	
 	*/
	bool isEmpty();

	/*	Get bonus army from successful exchange.
	*/
	int getExchangedArmies();

private:
	vector<Card> *cardDeck;
	// Number of bonus armies given after a successful exchange
	int *exchangeArmies;

	/**	Creates unshuffled deck equal to total number of countries
		@param total number of countries
	*/
	void createDeck(int totalCountries);

	/*	Shuffle the deck
	*/
	void shuffleDeck();
};

class Hand
{
public:
	//THIS WILL BE REMOVED AFTER TESTING PHASE
	int *armies;

	// constructor
	Hand();
	// destructor
	~Hand();

	/* Display cards in player's hand
	*/
	void showHand();

	/*	Add card to hand.
		@param Card object
	*/
	void addCardToHand(Card c);

	/*	Exchange 3 cards in hand for armies
		@param ownedCountries list (ref) to check matching country to give +2 armies
		@param deck object keeps track of how many armies to give
		@param isMandatory if exchange is mandatory if player has too many cards
		@return exchanged armies, if exchange cancelled, return 0
	*/
	int exchange(map<int,int>* ownedCountries, Deck* deck, bool isMandatory);

private:
	vector<Card> *playerHand;

	/*	Returns a string version of enum Card_Type
		@param Card type enum
	*/
	string getEnumString(Card_Type type);

	/*	Prompt user to choose which card from their hand to exchange.
		@param if exchange is mandatory
		@param number of cards that's already been chosen
	*/
	int getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen, int cardsToExchange[]);

	/*  Validate if the 3 selected cards are matchings or consecutives
		@param int i, j, k being the index of chosen card in hand
		@return true if valid, else false
	*/
	bool isValidExchangeCards(int i, int j, int k);

	/*	Check if exchanged cards match a country the player owns. If so, 
		prompt the player to choose which country to give +2 units.
		@param owned countries
		@param index of cards in player's hand that will be exchanged
	*/
	void giveBonusTwoArmies(map<int,int>* ownedCountries, int cardsToExchange[]);

	/*	Prompt user to choose which country that matches the exchanged cards 
		to receive +2 bonus armies.
		@param country ids that matches exchanged cards
	*/
	int getMatchingCountryChoice(vector<int> matchingCountries);
};

enum Card_Type
{
	INFANTRY,
	ARTILLERY,
	CAVALRY,
	WILD
};