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

private:
	vector<Card> *cardDeck;

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
	//THIS WILL BE REMOVED AFTER TESTING
	int *armies;

	Hand();
	/*	Destructor of Hand class.
	*/
	~Hand();

	/* Display cards in player's hand
	*/
	void showHand();

	/*	Add card to hand.
		@param Card object
	*/
	void addCardToHand(Card c);

	/*  Exchange 3 cards in hand for armies
		@param ownedCountries[] reference to list of owned countries
		@param isMandatory if exchange is mandatory
		@return if exchange was successful, false if exchange cancelled
	*/
	bool exchange(map<int,int> *ownedCountries, bool isMandatory);

private:
	vector<Card> *playerHand;

	/*	Returns a string version of enum Card_Type
		@param Card type enum
	*/
	string getEnumString(Card_Type type);

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
	void giveBonusTwoArmies(map<int,int> *ownedCountries, int cardsToExchange[]);
};

enum Card_Type
{
	INFANTRY,
	ARTILLERY,
	CAVALRY,
	WILD
};