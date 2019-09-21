#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Cards
{
public:

	/*	Constructor that creates and shuffles the deck
		@param total number of countries
	*/
	Cards(int totalCountries);

	/*** Struct ***/
	struct Card {
		enum Card_Type type;
		int countryId;
	};

	// TODO remove after testing
	struct DummyCountry {
		int id;
		int armies;
	};
	
	/*** Inner Class ***/
	class Hand 
	{
	public:

		/* Display cards in player's hand
		*/
		void showHand();

		/*  Exchange 3 cards in hand for armies
			@param ownedCountries[] reference to list of owned countries
			@param isMandatory if exchange is mandatory
			@return if exchange was successful, false if exchange cancelled
		*/
		bool exchange(vector<DummyCountry>* ownedCountries, bool isMandatory);

	private:
		std::vector<Card>* playerHand;

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
		// TODO modify after removing dummy country
		void giveBonusTwoArmies(vector<DummyCountry>* ownedCountries, int cardsToExchange[]);
	};

	// TODO remove after testing
	vector<DummyCountry>* ownedCountries;

	/*	Draws a card from top of deck
		@return Card object
	*/
	Card draw();

private:

	std::vector<Card>* deck; /// note vector is java's counterpart of arraylist

	/**	Creates unshuffled deck equal to total number of countries
		@param total number of countries
	*/
	void createDeck(int totalCountries);

	/*	Shuffle the deck
	*/
	void shuffleDeck();

	// TODO remove after testing
	void createDummyCountries(int totalCountries);
};

enum Card_Type {
	INFANTRY,
	ARTILLERY,
	CAVALRY,
	WILD
};