#pragma once

#include <vector>
#include <list>
#include <string>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"
#include "GameObservers.h"
#include "PlayerStrategies.h"

class Player : public Subject {

public:
	//constructor, destructor
	Player();
	Player(string name, Deck *deck, Map *mapPtr, PlayerStrategy *aStrategy, PhaseLog* aPhaseLog);
	Player(const Player &p);
	~Player();

	// Assignment operator
	const Player& operator =(const Player& rightSide);

	//accessors (some return copies not pointers to prevent modifications from outside).
	static int genNextID() { return (*currentGenId)++; };
	int getId() { return *id; };
	string getName() { return *name; };
	Hand* getHand() { return hand; };
	PlayerStrategy* getStrategy() { return strategy; };
	
	//mutators
	void setHand(Deck *deck) { hand = new Hand(deck, ownedCountries, strategy); };
	void setName(string newName) { *name = newName; };
	void setMap(Map *newMapPtr) { mapPtr = newMapPtr; };
	void setStrategy(PlayerStrategy* aStrategy) { strategy = aStrategy; hand->setPlayerStrategy(aStrategy); aStrategy->setPlayer(this); };

	void claimCountry(Country* country, int armies); // Used during attack(). Adds this country to the ones owned by the player, places on it this many armies
	Country* loseCountry(int id); // Used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned
	void setOwnedCountries(list<Country*> ownedCountries); //used in the startup phase of the game. Stores all owned countries and places 1 army on each.

	void reinforce(); // Get reinforcement armies and let player distribute given armies
	void attack(); //the player carries out a number of attacks
	void fortify(); //move a number of armies from an owned country to another owned neighboring country

	// service methods for external classes (i.e. GameEngine)
	int getNumOfOwnedCountries() { return ownedCountries->size(); }; // Get the number of Countries owned by the Player
	int getNumOfMapCountries() { return mapPtr->getCountries()->size(); }; // Get the total number of Countries in the Map
	// print out methods
	void displayOwnedCountries(); // Display owned countries' armies and continent id.

protected:
	map<int, Country*>* getOwnedCountries() { return ownedCountries; }; // Get a map of the Countries owned by the Player

private:
	static int* currentGenId; // Id used when creating new Players. Gets incremented each time a Player is created.
	int* id; // The Player's id
	string* name; // The Player's name
	map<int, Country*>* ownedCountries;  //maps each owned country id to a pointer to the Country object (used for n(1) searching)
	map<int,int>* numOfOwnedCountriesPerContinent;	// key: continent id, val: number of player's owned countries in that continent
	Map* mapPtr; // A pointer to the Map
	Hand* hand; // A pointer to the Player's hand
	Dice* dice; // A pointer to the Player's dice factory
	PlayerStrategy* strategy; // A pointer to a strategy to be used by the Player when making decisions. This strategy is not specific to this Player (may also be used by other Players)
	PhaseLog* phaseLogPtr; // A pointer to the Phase log

	// internal service methods
	int getCountryReinforcement(); // Get armies from total countries divided by 3
	int getContinentReinforcement(); // Get armies bonus from claiming entire continent
	void addOrRemoveArmies(int countryId, int armies); //armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.

	// Friends
	friend class AggressivePlayerStrategy;
	friend class PlayerStrategy;
	friend class BenevolentPlayerStrategy;
	friend class RandomPlayerStrategy;
	friend class CheaterPlayerStrategy;
};

class PhaseLog : public Subject {

public:
	PhaseLog();
	~PhaseLog();
	PhaseLog(const PhaseLog &pl);
	PhaseLog& operator = (const PhaseLog &pl);

	// Pass string msg and notify observers to print msg
	void printMsg(const string &s);
	// Accessor
	string* getMsg() { return msg; };

private:
	string* msg;
};
