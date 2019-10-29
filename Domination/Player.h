#pragma once

#include <vector>
#include <list>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

// Abstract base class
class Player {

private:
	static int* currentGenId;
	int* id;
	string* name;
	map<int, Country*>* ownedCountries;  //maps each owned country id to a pointer to the Country object
	map<int,int>* numOfOwnedCountriesPerContinent;	// key: continent id, val: number of player's owned countries in that continent
	Map* mapPtr;
	Hand* hand;
	Dice* dice;

protected:
	Hand* getHand() { return hand; };
	map<int, Country*>* getOwnedCountries() { return ownedCountries; };

	int getCountryReinforcement(); // Get armies from total countries divided by 3
	int getContinentReinforcement(); // Get armies bonus from claiming entire continent
	
public:
	//accessors (some return copies not pointers to prevent modifications from outside).
	int getId() { return *id; };
	string getName() { return *name; };
	
	//constructor, destructor
	Player();
	Player(string name, Deck *deck); 
	~Player(); 

	static int genNextID() { return (*currentGenId)++; }
	void setHand(Deck *deck) { hand = new Hand(deck, ownedCountries); }

	//service methods for reinforce, attack, fortify
	void setOwnedCountries(list<Country*> ownedCountries); //used in the startup phase of the game. Stores all owned countries and places 1 army on each.
	void claimCountry(Country* country, int armies); //used during attack(). Adds this country to the ones owned by the player, places on it this many armies. 
	Country* loseCountry(int id); //used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned. 
	void addOrRemoveArmies(int countryId, int armies); //armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.
	vector<int> rollDice(int howMany); //rolls this number of dice, returns dice results

	virtual void reinforce() = 0; // Get reinforcement armies and let player distribute given armies
	void attack(); //the player carries out a number of attacks
	void fortify(); //move a number of armies from an owned country to another owned neighboring country

	// service methods for external classes (i.e. GameEngine)
	int getNumOfOwnedCountries();
};

class PlayerHuman : public Player {

public:
	/// constructor, destructor
	PlayerHuman(int id, string name, Deck *deck);
	~PlayerHuman();

	/// override methods
	void reinforce(); // Get reinforcement armies and let player distribute given armies

private:

};

/*
class PlayerAI : public Player {

};
*/