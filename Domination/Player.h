#pragma once

#include <vector>
#include <list>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

// Abstract base class
class Player {

private:
	int* id;
	string* name;
	map<int, Country*>* ownedCountries;  //maps each owned country id to a pointer to the Country object
	Hand* hand;
	Dice* dice;

protected:
	Hand* getHand() { return hand; };
	map<int, Country*>* getOwnedCountries() { return ownedCountries; };
	

public:
	//accessors (they return copies not pointers to prevent modifications from outside).
	int getId() { return *id; };
	string getName() { return *name; };
	
	//constructor, destructor
	Player(int id, string name); 
	~Player(); 

	//service methods
	void setOwnedCountries(list<Country*> ownedCountries); //used in the startup phase of the game. Stores all owned countries and places 1 army on each.
	Country* loseCountry(int id); //used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned. 
	void claimCountry(Country* country, int armies); //used during attack(). Adds this country to the ones owned by the player, places on it this many armies. 
	void addOrRemoveArmies(int countryId, int armies); //armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.
	vector<int> rollDice(int howMany); //rolls this number of dice, returns dice results

	virtual void reinforce(Deck *deck) = 0; // Get reinforcement armies and let player distribute given armies
	void attack(); //the player carries out a number of attacks
	void fortify(); //move a number of armies from an owned country to another owned neighboring country

	int getCountryReinforcement(); // Get armies from total countries divided by 3
	int getContinentReinforcement(); // Get armies bonus from claiming entire continent
};

class PlayerHuman : public Player {

public:
	/// constructor, destructor
	PlayerHuman(int id, string name);
	~PlayerHuman();

	/// override methods
	void reinforce(Deck *deck); // Get reinforcement armies and let player distribute given armies

private:
	string getValidExchangeInput(); // Get valid input of string 'y' or 'n'
};

/*
class PlayerAI : public Player {

};
*/