#pragma once

#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

// Abstract base class
class Player {

private:
	int* id;
	string* name;
	map<int, Country*>* ownedCountries;  //maps each owned country id to a pointer to the Country object
	map<int, int>* ownedArmies;  //maps each owned country id to the number of armies owned on it
	Hand* hand;
	Dice* dice;

protected:
	Hand* getHand() { return hand; };
	map<int, Country*>* getOwnedCountries() { return ownedCountries; };
	

public:
	//accessors (they return copies not pointers to prevent modifications from outside).
	int getId() { return *id; };
	string getName() { return *name; };
	map<int, int> getOwnedArmies() { return *ownedArmies; };
	
	//constructor, destructor
	Player(int id, string name); 
	~Player(); 

	//service methods
	void setOwnedCountries(map<int, Country*> ownedCountries); //used in the set-up phase of the game. Also sets ownedArmies with 1 army on each owned country.
	void claimCountry(Country* country, int armies); //adds this country to the ones owned by the player, places on it this many armies 
	Country* loseCountry(int id); //returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned. 
	void addOrRemoveArmies(int countryId, int armies); //armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.
	vector<int> rollDice(int howMany); //rolls this number of dice, returns dice results

	virtual void reinforce(Deck *deck) = 0; // Get reinforcement armies and let player distribute given armies
	void attack(int from, int to); //an attack "from" an owned country "to" a country of another player
	void fortify(int from, int to, int armies); //move this many armies "from" an owned country "to" another owned country

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