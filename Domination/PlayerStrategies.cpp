#include "PlayerStrategies.h"
#include "Player.h"
#include "Cards.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <random>

using std::list;
using std::cout;
using std::cin;
using std::sort;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

// NOTE FOR MARKERS: some cout are not replaced by PhaseLog due to the printing format preference (UI less ugly)
// because each PhaseLog->printMsg starts a newline

//////////////////////////////////////////
/****** AGGRESSIVE PLAYER STRATEGY ******/

// Constructors, destructor
AggressivePlayerStrategy::AggressivePlayerStrategy() : strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(PhaseLog* phaseLogPtr) : PlayerStrategy(phaseLogPtr), strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &strategy) : PlayerStrategy(strategy), strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

// Assignment operator
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& rightSide)
{
	if (&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;
}

// Returns true when have cards >= 3
bool AggressivePlayerStrategy::ifPlayerWantsToExchange() 
{
	bool b = player->hand->getHandCount() >= 3;
	cout << "\nWould you like to exchange your cards? (y/n): ";
	if(b)	phaseLog->printMsg("y");
	else	phaseLog->printMsg("n");

	return b;	// attempt exchange whenever it's possible
}

// Automatically choose any cards in hands to exchange (has no user input)
int AggressivePlayerStrategy::promptExchangeForArmies(bool isMandatory)
{
	return exchangeAnyCardsForArmies();
}

/*	AI will choose to reinforce country with least armies
	@param country ids that matches exchanged cards
*/
void AggressivePlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries) 
{
	phaseLog->printMsg("\nCountries you own matches with your exchanged cards:");
		for (int id : *matchingCountries)
			phaseLog->printMsg(" > Country " + to_string(id));

	// choose strongest country
	int strongestCountryId = matchingCountries->at(0);
	int highestArmyCount = player->hand->ownedCountries->at(matchingCountries->at(0))->armies;
	for(int id : *matchingCountries) {
		if(player->hand->ownedCountries->at(id)->armies > highestArmyCount) {
			strongestCountryId = id;
			highestArmyCount = player->hand->ownedCountries->at(id)->armies;
		}
	}

	player->hand->ownedCountries->at(strongestCountryId)->armies += 2;
	phaseLog->printMsg("Choose a country to give 2 additional armies: " + to_string(strongestCountryId));
	cout << "---";
}

// Return any country with most armies
int AggressivePlayerStrategy::promptCountryToReinforce() 
{
	map<int, Country*>::iterator iter = player->ownedCountries->begin();
	int strongestCountryId = iter->first;
	int highestArmyCount = iter->second->armies;
	for (; iter != player->ownedCountries->end(); ++iter)
	{
		if(iter->second->armies > highestArmyCount) {
			strongestCountryId = iter->first;
			highestArmyCount = iter->second->armies;
		}
	}

	phaseLog->printMsg("Country: " + to_string(strongestCountryId));
	return strongestCountryId;
}

// Return all of given armies
int AggressivePlayerStrategy::promptNumOfArmiesToPlace(int totalArmies)
{
	cout << "Armies: " << totalArmies << endl;
	return totalArmies;
}

// Find the strongest Country owned by the Player (Country with most armies) that can attack other Countries
void AggressivePlayerStrategy::attackInit()
{
	Country* countryMaxArmies = nullptr;
	int maxArmies = 0;

	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		if (ownedCountry->second->armies > maxArmies) {
			list<int> neighbors = ownedCountry->second->neighbors;

			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				if (countryNeighbor->player->getId() != *(player->id) && ownedCountry->second->armies >= 2) {
					countryMaxArmies = ownedCountry->second;
					maxArmies = countryMaxArmies->armies;
					continue;
				}
			}
		}
	}

	strongestAttackingCountry = countryMaxArmies;
}

// Check if the strongest Country can still attack. If yes, return true.
bool AggressivePlayerStrategy::decideToAttack()
{
	// a country can be attacked if 1. it's a neighbour of an owned country
	// 2. it's not owned by the player
	// 3. the owned country has at least two armies
	if (strongestAttackingCountry != nullptr) {
		if (strongestAttackingCountry->armies >= 2) {
			list<int> neighbors = strongestAttackingCountry->neighbors;

			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				if (countryNeighbor->player->getId() != *(player->id)) {
					return true;
				}
			}
		}
	}

	return false;
}

// Choose the currently stored strongest Country
Country * AggressivePlayerStrategy::selectAttackingCountry()
{
	return strongestAttackingCountry;
}

// Choose the Country to attack using the attackingCountry
Country * AggressivePlayerStrategy::selectDefendingCountry(Country * attackingCountry)
{
	// The defendingCountry must 1. be a neighbour of the attacking country
	// 2. must not be owned by this Player
	Country* attackTarget;

	list<int> neighbors = attackingCountry->neighbors;
	for (auto const& neighbor : neighbors) {
		Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

		if (countryNeighbor->player->getId() != *(player->id)) {
			attackTarget = countryNeighbor;
			return attackTarget;
		}
	}

	throw exception("No countries can be an attack target for the provided attacking country.");
}

// Returns an integer - the number of dice to be rolled by the attacker
int AggressivePlayerStrategy::selectNumAttackDice(Country * attackingCountry)
{
	// The attack is allowed min: 1 die, max: (number of armies in the attacking Country - 1) dice
	// Will select the max number of dice to roll
	int max = attackingCountry->armies - 1;

	return max;
}

// Returns an integer - the number of dice to be rolled by the defender
int AggressivePlayerStrategy::selectNumDefenseDice(Country * defendingCountry)
{
	// The defense is allowed min: 1 die, max: 2 dice if the number of armies in defendingCountry >= 2
	// Will select the min number of dice to roll

	return 1;
}

// Returns the number of armies the attacker will move in newly conquered country
int AggressivePlayerStrategy::selectNumArmiesToMoveAfterAttackSuccess(Country * attackingCountry, Country * defendingCountry, int diceRolled)
{
	// The Player is allowed to move min: (number of dice rolled) armies, max: (number of armies in attacking Country - 1) armies
	// Will select the max number
	int max = attackingCountry->armies - 1;

	return max;
}

// Returns true if the Player can fortify (the Agressive Player fortifies by default if they can fortify)
bool AggressivePlayerStrategy::decideToFortify()
{
	return canFortify();
}

// Returns the strongest Country which can be fortified. Returns a nullptr if no Country owned by the Player can be fortified
Country * AggressivePlayerStrategy::selectFortifyDestination()
{
	// A Player can fortify a Country if one of the Country's neighbors is owned by the Player and has at least 2 armies on it
	// Will choose the Country with the most armies on it and which fulfills the above requirements
	Country* strongestFortifiableCountry = nullptr;
	int maxArmies = 0;

	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		if (ownedCountry->second->armies > maxArmies) {
			list<int> neighbors = ownedCountry->second->neighbors;

			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2) {
					strongestFortifiableCountry = ownedCountry->second;
					maxArmies = strongestFortifiableCountry->armies;
					continue;
				}
			}
		}
	}

	return strongestFortifiableCountry;
}

// Returns the Country neighbor of destination which is owned by the Player and has the most armies on it
// Returns a nullptr if no valid fortification source is found
Country * AggressivePlayerStrategy::selectFortifySource(Country * destination)
{
	// A Country can be a fortification source if it is owned by the Player, is a neighbor of the destination, and has at least 2 armies on it
	// Will choose the Country which fulfills the above requirement and has the most armies on it
	Country* strongestOwnedNeighbor = nullptr;
	int maxArmies = 2;

	list<int> neighbors = destination->neighbors;

	for (auto const& neighbor : neighbors) {
		Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

		if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= maxArmies) {
			strongestOwnedNeighbor = countryNeighbor;
			maxArmies = strongestOwnedNeighbor->armies;
		}
	}
	
	return strongestOwnedNeighbor;
}

// Returns the number of armies to move from source to destination
int AggressivePlayerStrategy::selectArmiesToMoveForFortification(Country * source, Country * destination)
{
	// The number of armies to move should be less than the number of armies on the source Country
	// Will choose the max number of armies that can be moved (armies on source - 1)
	return source->armies - 1;
}

///////////////////////////////////////////
/******* BENEVOLENT PLAYER STRATEGY ******/

// Constructors
BenevolentPlayerStrategy::BenevolentPlayerStrategy() {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(PhaseLog* phaseLogPtr) : PlayerStrategy(phaseLogPtr) {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &strategy) : PlayerStrategy(strategy) {}

// Destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

// Assignment operator
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& rightSide)
{
	if (&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;
}

// Returns true when have cards >= 3
bool BenevolentPlayerStrategy::ifPlayerWantsToExchange() 
{
	bool b = player->hand->getHandCount() >= 3;
	cout << "\nWould you like to exchange your cards? (y/n): ";
	if(b)	phaseLog->printMsg("y");
	else	phaseLog->printMsg("n");
	
	return b; // attempt exchange whenever it's possible
}

// Automatically choose any cards in hands to exchange (has no user input)
int BenevolentPlayerStrategy::promptExchangeForArmies(bool isMandatory) 
{
	return exchangeAnyCardsForArmies();
}

/*	AI will choose to reinforce country with least armies
	@param country ids that matches exchanged cards
*/
void BenevolentPlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries) 
{
	phaseLog->printMsg("\nCountries you own matches with your exchanged cards:");
		for (int id : *matchingCountries)
			phaseLog->printMsg(" > Country " + to_string(id));

	// choose weakest country
	int weakestCountryId = matchingCountries->at(0);
	int lowestArmyCount = player->hand->ownedCountries->at(matchingCountries->at(0))->armies;
	for(int id : *matchingCountries) {
		if(player->hand->ownedCountries->at(id)->armies < lowestArmyCount) {
			weakestCountryId = id;
			lowestArmyCount = player->hand->ownedCountries->at(id)->armies;
		}
	}

	player->hand->ownedCountries->at(weakestCountryId)->armies += 2;
	phaseLog->printMsg("Choose a country to give 2 additional armies: " + to_string(weakestCountryId));
	cout << "---";
}

// Return any country with least armies
int BenevolentPlayerStrategy::promptCountryToReinforce() 
{
	map<int, Country*>::iterator iter = player->ownedCountries->begin();
	int weakestCountryId = iter->first;
	int lowestArmyCount = iter->second->armies;
	for (; iter != player->ownedCountries->end(); ++iter)
	{
		if(iter->second->armies == 1) {	
			// 1 is lowest anyways so return first country without looping further
			weakestCountryId = iter->first;
			break;
		}
		else if(iter->second->armies < lowestArmyCount) {
			weakestCountryId = iter->first;
			lowestArmyCount = iter->second->armies;
		}
	}

	phaseLog->printMsg("Country: " + to_string(weakestCountryId));
	return weakestCountryId;
}

// Return all of given armies
int BenevolentPlayerStrategy::promptNumOfArmiesToPlace(int totalArmies)
{
	cout << "Armies: " << totalArmies << endl;
	return totalArmies;
}

// Returns false. A benevolent Player never attacks.
bool BenevolentPlayerStrategy::decideToAttack()
{
	return false;
}

// Returns the max of dice that can rolled for a given defending Country
int BenevolentPlayerStrategy::selectNumDefenseDice(Country * defendingCountry)
{
	// The defense is allowed min: 1 die, max: 2 dice if the number of armies in defendingCountry >= 2
	// Will choose the max number of dice
	if (defendingCountry->armies >= 2) {
		return 2;
	}
	
	return 1;
}

// Returns true if there exists a Country owned by the Player with less armies than other Countries owned and which can be fortified
// by a neighboring Country
bool BenevolentPlayerStrategy::decideToFortify()
{
	bool sameAmountArmiesEverywhere = true;
	int minArmies = player->ownedCountries->begin()->second->armies;

	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		if (ownedCountry->second->armies != minArmies) {
			sameAmountArmiesEverywhere = false;
		}
	}

	if (!sameAmountArmiesEverywhere) {
		minArmies++;

		for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
			if (ownedCountry->second->armies < minArmies) {
				list<int> neighbors = ownedCountry->second->neighbors;

				for (auto const& neighbor : neighbors) {
					Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

					// The number of armies in the neighbor should also be higher than the number of armies in the destination
					if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2 && countryNeighbor->armies > ownedCountry->second->armies) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

// Returns the weakest Country which can be fortified. Returns a nullptr if no Country owned by the Player can be fortified
Country * BenevolentPlayerStrategy::selectFortifyDestination()
{
	// A Player can fortify a Country if one of the Country's neighbors is owned by the Player and has at least 2 armies on it
	// Will choose the Country with the least armies on it and which fulfills the above requirements
	Country* weakestFortifiableCountry = nullptr;

	// Will initialize min armies to the first found owned Country's number of armies + 1.
	// This ensures that 1. the initial amount is not too low and 2. the first found owned Country will still be a potential candidate
	int minArmies = player->ownedCountries->begin()->second->armies + 1;

	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		if (ownedCountry->second->armies < minArmies) {
			list<int> neighbors = ownedCountry->second->neighbors;

			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				// The number of armies in the neighbor should also be higher than the number of armies in the destination
				if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2 && countryNeighbor->armies > ownedCountry->second->armies) {
					weakestFortifiableCountry = ownedCountry->second;
					minArmies = weakestFortifiableCountry->armies;
					continue;
				}
			}
		}
	}
	return weakestFortifiableCountry;
}

// Returns the Country neighbor of destination which is owned by the Player and has the most armies on it
// Returns a nullptr if no valid fortification source is found
Country * BenevolentPlayerStrategy::selectFortifySource(Country * destination)
{
	// A Country can be a fortification source if it is owned by the Player, is a neighbor of the destination, and has at least 2 armies on it
	// Will choose the Country which fulfills the above requirement and has the most armies on it
	Country* strongestOwnedNeighbor = nullptr;
	int maxArmies = 2;
	list<int> neighbors = destination->neighbors;

	for (auto const& neighbor : neighbors) {
		Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

		if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= maxArmies) {
			strongestOwnedNeighbor = countryNeighbor;
			maxArmies = strongestOwnedNeighbor->armies;
		}
	}
	return strongestOwnedNeighbor;
}

// Returns the number of armies to move from source to destination
int BenevolentPlayerStrategy::selectArmiesToMoveForFortification(Country * source, Country * destination)
{
	// The number of armies to move should be less than the number of armies on the source Country
	// Will choose the number of armies that can be moved that will allow source and destination to have an equal number of 
	// armies (+/- 1)
	int armiesToMove = 0;
	if (source->armies > destination->armies) {
		int armyGap = source->armies - destination->armies;
		armiesToMove = armyGap / 2 + armyGap % 2;
	}

	return armiesToMove;
}

//////////////////////////////////////////
/***** RANDOM (AI) PLAYER STRATEGY ******/
// Default constructor
RandomPlayerStrategy::RandomPlayerStrategy() {}
// Overloaded constructors
RandomPlayerStrategy::RandomPlayerStrategy(PhaseLog* phaseLogPtr) : PlayerStrategy(phaseLogPtr) {}
// Copy constructor
RandomPlayerStrategy::RandomPlayerStrategy(const RandomPlayerStrategy& strategy) : PlayerStrategy(strategy) {}
// Destructor 
RandomPlayerStrategy::~RandomPlayerStrategy() {}
// Assignment operator
RandomPlayerStrategy& RandomPlayerStrategy::operator =(const RandomPlayerStrategy& rightSide) {
	if(&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;
}

// If have >= 3 cards, make a 50/50 decision to exchange or not.
bool RandomPlayerStrategy::ifPlayerWantsToExchange()
{
	int b = 0;
	if(player->hand->getHandCount() >= 3) {
		b = genRandomNum(0, 1);
	}

	cout << "\nWould you like to exchange your cards? (y/n): ";
	if(b)	phaseLog->printMsg("y");
	else	phaseLog->printMsg("n");
	
	return b;
	
}

// Automatically choose any cards in hands to exchange (has no user input)
int RandomPlayerStrategy::promptExchangeForArmies(bool isMandatory) 
{
	return exchangeAnyCardsForArmies();
}

// Choose random country to give +2 bonus
void RandomPlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries)
{
	phaseLog->printMsg("\nCountries you own matches with your exchanged cards:");
		for (int id : *matchingCountries)
			phaseLog->printMsg(" > Country " + to_string(id));

	// choose randomly 1 of the matching countries
	int ranNum = genRandomNum(0, matchingCountries->size()-1);
	int randomCountryId = matchingCountries->at(ranNum);

	player->hand->ownedCountries->at(randomCountryId)->armies += 2;
	phaseLog->printMsg("Choose a country to give 2 additional armies: " + to_string(randomCountryId));
	cout << "---";
}

// Return a random country player owns.
int RandomPlayerStrategy::promptCountryToReinforce() 
{
	vector<int> ownedIds;
	// get all owned country's id
	map<int, Country*>::iterator iter = player->ownedCountries->begin();
	for (; iter != player->ownedCountries->end(); ++iter)
		ownedIds.push_back(iter->first);

	int rand = genRandomNum(0, ownedIds.size()-1);
	phaseLog->printMsg("Country: " + to_string(ownedIds.at(rand)));
	return ownedIds.at(rand);
}

// Return random amount of armies to place.
int RandomPlayerStrategy::promptNumOfArmiesToPlace(int totalArmies)
{
	int rand = genRandomNum(1, totalArmies);
	cout << "Armies: " << rand << endl;
	return rand;
} 

bool RandomPlayerStrategy::decideToAttack() {
	return genRandomNum(0,1);
}

Country* RandomPlayerStrategy::selectAttackingCountry() {
	vector<int> possibleAttackers; // will store ids of potential attackers
	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) { // loop through player owned countries 
		list<int> neighbors = ownedCountry->second->neighbors; // get neighbors of each country 

		for (auto const& neighbor : neighbors) { // loop through neighbors 
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor); // get neighbor id

			if (countryNeighbor->player->getId() != *(player->id) && ownedCountry->second->armies >= 2) { // if neighbor doesn't belong to player
				possibleAttackers.push_back(ownedCountry->second->id); // add the country to potential attackers 
			}
		}		
	}
	
	int index = genRandomNum(0, possibleAttackers.size() - 1); // get a random index 
	int id = possibleAttackers.at(index); // get a random id 
	Country *country = player->mapPtr->getCountryById(id); //gets a random country from possbilities
	return country;
}

Country* RandomPlayerStrategy::selectDefendingCountry(Country* attackingCountry) { 
	Country* attackTarget; // the target 
	Country* countryNeighbor; // the neighbor
	list<int> neighbors = attackingCountry->neighbors; // neighbors of attacker 
	vector<int> possibleTargets; //possible target if neighbor of attacking country

	for (auto const& neighbor : neighbors) { // loop through neighbors 
		countryNeighbor = player->mapPtr->getCountryById(neighbor);

		if (countryNeighbor->player->getId() != *(player->id)) { 
			possibleTargets.push_back(neighbor); //if neighbor isn't attacker owned country, add to possible targets
		}
	}
	
	int index = genRandomNum(0, possibleTargets.size() - 1); // get a random index 
	int id = possibleTargets.at(index); // get a random id
	attackTarget = player->mapPtr->getCountryById(id); //get a random country from possible targets
	return attackTarget;
	throw exception("No countries can be an attack target for the provided attacking country.");
}

int RandomPlayerStrategy::selectNumAttackDice(Country* attackingCountry) {
	// The attack is allowed min: 1 die, max: (number of armies in the attacking Country - 1) dice
	int max = attackingCountry->armies - 1;

	return genRandomNum(1, max);
}

int RandomPlayerStrategy::selectNumDefenseDice(Country* defendingCountry) {
	// The defense is allowed min: 1 die, max: 2 dice if the number of armies in defendingCountry >= 2

	return genRandomNum(1,2);
}

int RandomPlayerStrategy::selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled) {
	// The Player is allowed to move min: (number of dice rolled) armies, max: (number of armies in attacking Country - 1) armies
	int min = diceRolled;
	int max = attackingCountry->armies - 1;

	return genRandomNum(min, max);
}

bool RandomPlayerStrategy::decideToFortify() {
	if (canFortify()) {
		return genRandomNum(0, 1); // if possible to fortify, decide randomly
	}
	else
		return false; // otherwise won't fortify
}
Country* RandomPlayerStrategy::selectFortifyDestination() {
	// A Player can fortify a Country if one of the Country's neighbors is owned by the Player and has at least 2 armies on it
	Country* countryToFortify;
	Country* countryNeighbor;
	vector<int> possibleChoices; // stores ids of possbile choices 

	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) { // loop through owned countries 
		
		list<int> neighbors = ownedCountry->second->neighbors; // list of neighbors of each owned country 

		for (auto const& neighbor : neighbors) { // loop through neighbors 
			countryNeighbor = player->mapPtr->getCountryById(neighbor); // get neighbor by id 

			if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2) { // if neighbor belongs to player
				possibleChoices.push_back(ownedCountry->second->id); // add it to possible choices 
			}
		}	
	}
	int index = genRandomNum(0, possibleChoices.size() - 1); // get a random index 
	int id = possibleChoices.at(index); // get a random id 
	countryToFortify = player->mapPtr->getCountryById(id); // gets a country by the random id 
	return countryToFortify;
}

Country* RandomPlayerStrategy::selectFortifySource(Country* destination) {
	// A Country can be a fortification source if it is owned by the Player, is a neighbor of the destination, and has at least 2 armies on it
	Country* fortifySource; 
	Country* countryNeighbor;
	vector<int> possibleChoices; // stores possible choices 

	list<int> neighbors = destination->neighbors; // gets neighbors ids of destination country 

	for (auto const& neighbor : neighbors) { // loops through neighbor ids 
		countryNeighbor = player->mapPtr->getCountryById(neighbor); // gets neighbor by id 

		if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2) { // if owned by player and has >= 2 armies 
			possibleChoices.push_back(neighbor); // add to list of possibilities 
		}
	}
	int index = genRandomNum(0, possibleChoices.size() - 1); // get random index 
	int id = possibleChoices.at(index); // get random id from possibilites 
	fortifySource = player->mapPtr->getCountryById(id); // get source by id 
	return fortifySource;
}

int RandomPlayerStrategy::selectArmiesToMoveForFortification(Country* source, Country* destination) {
	// The number of armies to move should be less than the number of armies on the source Country	
	int max = source->armies - 1;
	return genRandomNum(0, max);
}

//////////////////////////////////////////
/***** CHEATER (AI) PLAYER STRATEGY ******/
// Default constructor
CheaterPlayerStrategy::CheaterPlayerStrategy() {}
// Overloaded constructors
CheaterPlayerStrategy::CheaterPlayerStrategy(PhaseLog* phaseLogPtr) : PlayerStrategy(phaseLogPtr) {}
// Copy constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& strategy) : PlayerStrategy(strategy) {}
// Destructor 
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}
// Assignment operator
CheaterPlayerStrategy& CheaterPlayerStrategy::operator =(const CheaterPlayerStrategy& rightSide) {
	if(&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;
}

// Return true if have >= 3 cards
bool CheaterPlayerStrategy::ifPlayerWantsToExchange()
{
	bool b = player->hand->getHandCount() >= 3;
	cout << "\nWould you like to exchange your cards? (y/n): ";
	if(b)	phaseLog->printMsg("y");
	else	phaseLog->printMsg("n");
	
	return b; // attempt exchange whenever it's possible
}

// Automatically choose any cards in hands to exchange (has no user input)
int CheaterPlayerStrategy::promptExchangeForArmies(bool isMandatory) 
{
	return exchangeAnyCardsForArmies();
}

// Give bonus to ALL matching countries
void CheaterPlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries)
{
	phaseLog->printMsg("\nCountries you own matches with your exchanged cards:");
		for (int id : *matchingCountries)
			phaseLog->printMsg(" > Country " + to_string(id));

	phaseLog->printMsg("Choose a country to give 2 additional armies: ");
	for(int id : *matchingCountries) {
		player->hand->ownedCountries->at(id)->armies += 2;
		cout << id << " ";
	}
	cout << "\n---";
}

/*	Prompt user to choose which countries to distribute their reinforcement armies
	@param int: total armies from reinforcement to distribute
*/
void CheaterPlayerStrategy::distributeArmies(int totalArmies)
{
	int countryInput, armiesInput, i = 0;
	map<int,Country*>::iterator iter = player->ownedCountries->begin();

	player->displayOwnedCountries();
	phaseLog->printMsg("Double your armies in every country to display true supremacy?: Yes.");
	for(; iter != player->ownedCountries->end(); ++iter) {
		player->ownedCountries->at(iter->first)->armies = (2 * iter->second->armies);
	}
	player->displayOwnedCountries();
}

// Conquers all the neighbors of all countries owned by the player
void CheaterPlayerStrategy::cheaterAttack()
{
	vector<Country*> countriesToConquer = vector<Country*>();
	vector<int> armiesToAdd = vector<int>();
	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		list<int> neighbors = ownedCountry->second->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->player->getId() != *(player->id)) {
				countriesToConquer.push_back(countryNeighbor);
				armiesToAdd.push_back(ownedCountry->second->armies);
			}
		}
	}

	for (int i = 0; i < countriesToConquer.size(); i++) {
		phaseLog->printMsg("Player " + player->getName() + " conquered Country " + countriesToConquer.at(i)->name + " (" + to_string(countriesToConquer.at(i)->id) + ") and placed " + to_string(armiesToAdd.at(i)) + " armies on it");
		player->claimCountry(countriesToConquer.at(i), armiesToAdd.at(i));
	}
}

// Doubles the number of armies on all Countries owned by the player which have enemy neighbors
void CheaterPlayerStrategy::cheaterFortify()
{
	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		list<int> neighbors = ownedCountry->second->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->player->getId() != *(player->id)) {
				ownedCountry->second->armies *= 2;
				phaseLog->printMsg("Player " + player->getName() + " doubled the armies in Country " + ownedCountry->second->name + " (" + to_string(ownedCountry->second->id) + ")\nCountry " + ownedCountry->second->name + " (" + to_string(ownedCountry->second->id) + ") now has " + to_string(ownedCountry->second->armies) + " armies");
			}
		}
	}
}

////////////////////////////////////////////////
/****** DEFAULT (HUMAN) PLAYER STRATEGY *******/

// Constructors
PlayerStrategy::PlayerStrategy() : player(nullptr), phaseLog(nullptr) {}
PlayerStrategy::PlayerStrategy(PhaseLog* phaseLogPtr) : player(nullptr), phaseLog(phaseLogPtr) {}
PlayerStrategy::PlayerStrategy(const PlayerStrategy &strategy) : player(strategy.player), phaseLog(strategy.phaseLog) {}

// Destructor
PlayerStrategy::~PlayerStrategy() {}

// Assignment operator
PlayerStrategy& PlayerStrategy::operator =(const PlayerStrategy& rightSide) {
	if (&rightSide != this) {
		player = rightSide.player;
		phaseLog = rightSide.phaseLog;
	}
	return *this;
}

//	Return if player wants to exchange
bool PlayerStrategy::ifPlayerWantsToExchange() 
{
	string input;
	do {
		cout << "\nWould you like to exchange your cards? (y/n): ";
		cin >> input;

		if(input.compare("y") == 0 || input.compare("n") == 0)  // 0 means equal
			break;
		else
			phaseLog->printMsg("\nInput must be 'y' or 'n'");
	} 
	while (true);

	return (input.compare("y") == 0);
}

/*	Exchange 3 cards for armies.
	@param if exchange action is mandatory
	@return exchanged armies, if exchange cancelled, return 0
*/
int PlayerStrategy::promptExchangeForArmies(bool isMandatory) 
{
	int numOfCardsChosen = 0;
	int cardsToExchangeIndex[3];

	do {
		/// get player input
		int selectedCardIndex = getPlayersCardOfChoice(isMandatory, numOfCardsChosen, cardsToExchangeIndex);

		if (selectedCardIndex == 0 && !isMandatory) {	/// if player wants to cancel
			phaseLog->printMsg("\nExchange action cancelled.");
			break;
		}
		else {	/// store chosen card
			cardsToExchangeIndex[numOfCardsChosen] = selectedCardIndex - 1;
			numOfCardsChosen++;
		}

		if (numOfCardsChosen == 3) {
			if (player->hand->isValidExchangeCards(cardsToExchangeIndex[0], cardsToExchangeIndex[1], cardsToExchangeIndex[2]))
			{
				/// any elements with an index higher than the removed element's gets their index shifted by one (minus one).
				/// sort index with descending order so index doesnt shift when removing Card object from playerHand vector
				sort(begin(cardsToExchangeIndex), end(cardsToExchangeIndex), greater<int>());
				tradeInCards(cardsToExchangeIndex);
				/// exchange is successful
				return player->hand->deck->getExchangedArmies();
			}
			else {
				phaseLog->printMsg("\nCannot exchange with these cards. Must be a matching or of consecutive types.");
				numOfCardsChosen = 0;
				cardsToExchangeIndex[0] = -5; cardsToExchangeIndex[1] = -5; cardsToExchangeIndex[2] = -5; // clear previous chosen cards
				// note for others: do not make it -1 
			}
		}
	} while (numOfCardsChosen != 3);

	return 0;
}

/*	Prompt user to choose which card from their hand to exchange.
	@param if exchange is mandatory
	@param number of cards that's already been chosen
	@param index of already selected cards to exchange
*/
int PlayerStrategy::getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen, int cardsToExchangeIndex[]) 
{
	int selectedCardIndex;
	do {
		cout << "Card " << (numOfCardsChosen + 1) << ": ";

		cin >> selectedCardIndex;
		if (!cin.good())	/// !good() when input doesnt match declared type
		{
			phaseLog->printMsg("\nInvalid number input. Please try again.");
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else if (selectedCardIndex == 0 && isMandatory)
			phaseLog->printMsg("\nYou've reached the card limit and must exchange.");
		else if (selectedCardIndex < 0 || selectedCardIndex > player->hand->playerHand->size())
			phaseLog->printMsg("\nYour choice must be within your hand's cards.");
		else if (selectedCardIndex == cardsToExchangeIndex[0]+1 || 
				selectedCardIndex == cardsToExchangeIndex[1]+1)
			phaseLog->printMsg("\nYou have already selected this card.");
		else
			break;
	} 
	while (true);

	return selectedCardIndex;
}

/*	Perform successful exchange and return armies. If have cards matching owned countries, get bonus armies.
	@param 3 card indexes to exchange
	@return armies from exchange (excluding the matching bonus)
*/
void PlayerStrategy::tradeInCards(int cardsToExchange[]) 
{
	/// give bonus +2 armies if cards match owned countries
	vector<int>* matchingCountries = new vector<int>();
	
	for (int i = 0; i < 3; i++) {	/// loop each exchanged cards if matches owned countries	
		int countryId = player->hand->playerHand->at(cardsToExchange[i]).countryId;
		if(player->hand->ownedCountries->count(countryId) > 0)	/// 0 if not an element, 1 if is
			{ matchingCountries->push_back(countryId);	}	/// store country by reference
	}
	/// if have matching, prompt player input to choose which country to give +2 units
	if (!matchingCountries->empty()) 
		distributeExchangeBonus(matchingCountries); // calls derived func of distributeExchangeBOnus if called from subclass
	delete matchingCountries;

	/// remove exchanged cards from hand (note already sorted by descending order)
	player->hand->playerHand->erase(player->hand->playerHand->begin() + cardsToExchange[0]);
	player->hand->playerHand->erase(player->hand->playerHand->begin() + cardsToExchange[1]);
	player->hand->playerHand->erase(player->hand->playerHand->begin() + cardsToExchange[2]);
}

/*	Prompt user to choose which country that matches the exchanged cards to receive +2 bonus armies.
	@param country ids that matches exchanged cards
*/
void PlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries) 
{
	phaseLog->printMsg("\nCountries you own matches with your exchanged cards:");
		for (int id : *matchingCountries)
			phaseLog->printMsg(" > Country " + to_string(id));

	int selectedCountryId;
	bool validInput = false;
	do {
		cout << "Choose a country to give 2 additional armies: ";
		selectedCountryId = promptNumberInput();

		for(int id : *matchingCountries) {
			cout << "ID: " << id << " SELECT: " << selectedCountryId;
			if(selectedCountryId == id)
				validInput = true;
		}
		phaseLog->printMsg("\nPlease choose the countries given.");
	} 
	while (!validInput);
	cout << "\n---";

	player->hand->ownedCountries->at(selectedCountryId)->armies += 2;
}

/*	Prompt user to choose which countries to distribute their reinforcement armies
	@param int: total armies from reinforcement to distribute
*/
void PlayerStrategy::distributeArmies(int totalArmies)
{
	int countryInput, armiesInput, i = 0;

	player->displayOwnedCountries();
	while(totalArmies > 0) 
	{
		phaseLog->printMsg("---\nYou have " + to_string(totalArmies) + " armies" + 
							(i++? " left." : ". Deploy your armies!")); // ++ to show different msg the second time
		countryInput = promptCountryToReinforce();
		armiesInput = promptNumOfArmiesToPlace(totalArmies);
		player->addOrRemoveArmies(countryInput, armiesInput);
		totalArmies -= armiesInput;
	}
	player->displayOwnedCountries();
}

// prompt which country to reinforce
int PlayerStrategy::promptCountryToReinforce() 
{
	int countryInput;
	do {
		cout << "Country: ";
		countryInput = promptNumberInput();
		if(player->ownedCountries->count(countryInput) == 0)
			phaseLog->printMsg("\nYou do not own this country.");
		else break;
	} while(true);
	return countryInput;
}

// prompt which country to reinforce
int PlayerStrategy::promptNumOfArmiesToPlace(int totalArmies)
{
	int armiesInput;
	do {
		cout << "Armies: ";
		armiesInput = promptNumberInput();
		if (armiesInput > totalArmies || armiesInput < 0) 
			phaseLog->printMsg("\nYou must enter armies in the range given.");
		else break;
	} while(true);
	return armiesInput;
}

/*	Prompt user to input a NUMBER only.
	@return valid number input
*/
int PlayerStrategy::promptNumberInput() 
{
	int n;
	do {
		cin >> n;
		if (!cin.good()) {	/// !good() when input doesnt match declared type
			phaseLog->printMsg("\nInvalid number input. Please try again.");
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else break;
	} while (true);

	return n;
}

// Does any action required to be done before deciding to attack
void PlayerStrategy::attackInit()
{
	// do nothing
	return;
}

// Ask user to choose whether to attack
bool PlayerStrategy::decideToAttack()
{
	bool canAttack = false;
	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		// continue attacking until there is no more countries that can be attacked
		// a country can be attacked if 1. it's a neighbour of an owned country
		// 2. it's not owned by the player
		// 3. the owned country has at least two armies
		list<int> neighbors = ownedCountry->second->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->player->getId() != *(player->id) && ownedCountry->second->armies >= 2) {
				canAttack = true;
			}
		}
	}

	if (!canAttack) {
		cout << endl << "None of your countries can attack another country." << endl;
		cout << "Your attack phase will be skipped." << endl;

		return false;
	}

	cout << endl << "Would you like to attack? Enter 0 for no or 1 for yes." << endl;

	int choice = -1;
	cin >> choice;

	while (!cin.good() || (choice != 0 && choice != 1)) {
		cout << endl << "This input is wrong. Please enter 0 for no or 1 for yes.";
		cin >> choice;
	}

	cout << endl;

	return choice;
}

// Asks user to choose a country to attack from
Country * PlayerStrategy::selectAttackingCountry()
{
	int countryId;
	Country* selectedCountry = nullptr;
	bool validCountryId = false;

	do {
		cout << endl << "Please enter the id of the country to attack from." << endl;
		cin >> countryId;

		if (!cin.good()) {
			cout << "The id should be an integer." << endl;
			continue;
		}

		try {
			selectedCountry = player->mapPtr->getCountryById(countryId);
			
			if (selectedCountry->armies < 2) {
				cout << "The country you selected does not have enough armies to attack (minimum armies required is 2)." << endl;
				continue;
			}

			list<int> neighbors = selectedCountry->neighbors;
			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				if (countryNeighbor->player->getId() != *(player->id)) {
					validCountryId = true;
					break;
				}
			}

			if (!validCountryId) {
				cout << "The country you selected has no countries it can attack." << endl;
			}
		}
		catch (exception e) {
			cout << "None of your countries has this id." << endl;
			continue;
		}
	} while (!validCountryId);

	return selectedCountry;
}

// Asks user to choose the Country to attack using attackCountry
Country * PlayerStrategy::selectDefendingCountry(Country * attackingCountry)
{
	// display info on neighbors of attackingCountry
	int countryId;
	Country* selectedCountry = nullptr;
	bool validCountryId = false;

	do {
		cout << endl << "Please enter the id of the country you wish to attack." << endl;
		cin >> countryId;

		if (!cin.good()) {
			cout << "The id should be an integer." << endl;
			continue;
		}

		try {
			selectedCountry = player->mapPtr->getCountryById(countryId);

			list<int> neighbors = selectedCountry->neighbors;
			bool notNeighbors = true;
			for (auto const& neighbor : neighbors) {
				Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

				if (countryNeighbor->id == attackingCountry->id) {
					notNeighbors = false;
					if (selectedCountry->player->getId() == player->getId()) {
						cout << "You cannot attack a country you own." << endl;
					}
					else {
						validCountryId = true;
					}
				}
			}

			if (notNeighbors) {
				cout << "You must select a country that is a neighbor of your attacking country." << endl;
			}
		}
		catch (exception e) {
			cout << "No countries have this id." << endl;
			continue;
		}
	} while (!validCountryId);

	return selectedCountry;
}

// Asks user how many dice they want to use for the attack
int PlayerStrategy::selectNumAttackDice(Country * attackingCountry)
{
	// The attack is allowed min: 1 die, max: (number of armies in the attacking Country - 1) dice
	int selectedNumAttackDice;
	const int maxNumAttackDice = attackingCountry->armies - 1;
	bool validNumAttackDice = false;

	do {
		cout << endl << "Please enter the number of dice you wish to roll for attack [";
		cout << MIN_NUM_ATTACK_DICE << "-" << maxNumAttackDice << "]." << endl;
		cin >> selectedNumAttackDice;

		if (!cin.good()) {
			cout << "The number of dice should be an integer." << endl;
			continue;
		}

		if (selectedNumAttackDice < MIN_NUM_ATTACK_DICE) {
			cout << "You should roll at least " << MIN_NUM_ATTACK_DICE << " dice." << endl;
			continue;
		}

		if (selectedNumAttackDice > maxNumAttackDice) {
			cout << "You cannot roll more than " << maxNumAttackDice << " dice." << endl;
			continue;
		}

		validNumAttackDice = true;
	} while (!validNumAttackDice);

	return selectedNumAttackDice;
}

// Asks user for how many dice they want to roll for defending from an attack
int PlayerStrategy::selectNumDefenseDice(Country * defendingCountry)
{
	// The defense is allowed min: 1 die, max: 2 dice if the number of armies in defendingCountry >= 2
	int selectedNumDefenseDice;
	bool validNumDefenseDice = false;
	
	do {
		cout << endl << "Player " << defendingCountry->player->getName() << ", ";
		cout << "please enter the number of dice you wish to roll for defense [";
		cout << MIN_NUM_DEFENSE_DICE << "-" << MAX_NUM_DEFENSE_DICE << "]." << endl;
		cin >> selectedNumDefenseDice;

		if (!cin.good()) {
			cout << "The number of dice should be an integer." << endl;
			continue;
		}

		if (selectedNumDefenseDice < MIN_NUM_DEFENSE_DICE) {
			cout << "You should roll at least " << MIN_NUM_DEFENSE_DICE << " dice." << endl;
			continue;
		}

		if (selectedNumDefenseDice > MAX_NUM_DEFENSE_DICE) {
			cout << "You cannot roll more than " << MAX_NUM_DEFENSE_DICE << " dice." << endl;
			continue;
		}

		if (selectedNumDefenseDice == MAX_NUM_DEFENSE_DICE && defendingCountry->armies < MAX_NUM_DEFENSE_DICE) {
			cout << "You can only roll " << MAX_NUM_DEFENSE_DICE << " dice if your defending country has at least ";
			cout << MAX_NUM_DEFENSE_DICE << " armies on it." << endl;
			continue;
		}

		validNumDefenseDice = true;
	} while (!validNumDefenseDice);

	return selectedNumDefenseDice;
}

// Asks user for how many armies they want to move from the attackingCountry into the newly claimed Country
int PlayerStrategy::selectNumArmiesToMoveAfterAttackSuccess(Country * attackingCountry, Country * defendingCountry, int diceRolled)
{
	// The Player is allowed to move min: (number of dice rolled) armies, max: (number of armies in attacking Country - 1) armies

	int selectedNumArmies;
	int max = attackingCountry->armies - 1;
	int min = diceRolled;
	bool validNumArmies = false;

	if (diceRolled > max) {
		min = max;
	}

	if (min == max) {
		cout << "You are required to move " << min << " armies from country " << attackingCountry->name << " into ";
		cout << "country " << defendingCountry->name << ".";
		return min;
	}

	do {
		cout << endl << "Please enter the number of armies you wish to move from country " << attackingCountry->name;
		cout << " to country " << defendingCountry->name << " [" << min << "-" << max << "]." << endl;
		cin >> selectedNumArmies;

		if (!cin.good()) {
			cout << "The number of armies to move should be an integer." << endl;
			continue;
		}

		if (selectedNumArmies < min) {
			cout << "You should move at least " << min << " armies." << endl;
			continue;
		}

		if (selectedNumArmies > max) {
			cout << "You cannot move more than " << max << " armies." << endl;
			continue;
		}

		validNumArmies = true;
	} while (!validNumArmies);

	return selectedNumArmies;
}

void PlayerStrategy::cheaterAttack()
{
	// do nothing
}

// Returns true if the Player has a Country that can be fortified
bool PlayerStrategy::canFortify()
{
	// A Player can fortify a Country if one of the Country's neighbors is owned by the Player and has at least 2 armies on it
	for (auto ownedCountry = player->ownedCountries->begin(); ownedCountry != player->ownedCountries->end(); ownedCountry++) {
		list<int> neighbors = ownedCountry->second->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2) {
				return true;
			}
		}
	}

	return false;
}

// Returns true if destination has neighbor which belongs to the same Player and which has more than 1 army on it
bool PlayerStrategy::isValidFortifyDestination(Country * destination)
{
	if (destination->player->id == player->id) {
		list<int> neighbors = destination->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->player->getId() == *(player->id) && countryNeighbor->armies >= 2) {
				return true;
			}
		}
	}
	
	return false;
}

// Returns true if source is a neighbor of destination, is owned by the same Player and has at least two armies on it
bool PlayerStrategy::isValidFortifiySource(Country * destination, Country * source)
{
	if (source->player->getId() == destination->player->getId() && source->armies >= 2) {
		list<int> neighbors = destination->neighbors;

		for (auto const& neighbor : neighbors) {
			Country* countryNeighbor = player->mapPtr->getCountryById(neighbor);

			if (countryNeighbor->id == source->id) {
				return true;
			}
		}
	}

	return false;
}

// Asks user if they want to fortify a Country
bool PlayerStrategy::decideToFortify()
{
	if (!canFortify()) {
		cout << endl << "None of your countries can be fortified." << endl;
		cout << "Your fortification phase will be skipped." << endl;

		return false;
	}

	cout << endl << "Would you like to fortify a Country? Enter 0 for no or 1 for yes." << endl;

	int choice = -1;
	cin >> choice;

	while (!cin.good() || (choice != 0 && choice != 1)) {
		cout << endl << "This input is wrong. Please enter 0 for no or 1 for yes.";
		cin >> choice;
	}

	cout << endl;

	return choice;
}

// Asks user for which Country they want to fortify
Country * PlayerStrategy::selectFortifyDestination()
{
	// A Player can fortify a Country if one of the Country's neighbors is owned by the Player and has at least 2 armies on it
	int countryId;
	Country* selectedCountry = nullptr;
	bool validCountryId = false;

	do {
		cout << endl << "Please enter the id of the country you wish to fortify." << endl;
		cin >> countryId;

		if (!cin.good()) {
			cout << "The id should be an integer." << endl;
			continue;
		}

		try {
			selectedCountry = player->mapPtr->getCountryById(countryId);

			if (isValidFortifyDestination(selectedCountry)) {
				validCountryId = true;
			}
			else {
				// not valid
			}
		}
		catch (exception e) {
			cout << "No countries have this id." << endl;
			continue;
		}
	} while (!validCountryId);

	return selectedCountry;
}

// Asks user for which Country they to want to transfer armies into destination
Country * PlayerStrategy::selectFortifySource(Country * destination)
{
	// A Country can be a fortification source if it is owned by the Player, is a neighbor of the destination, and has at least 2 armies on it
	int countryId;
	Country* selectedCountry = nullptr;
	bool validCountryId = false;

	do {
		cout << endl << "Please enter the id of the country from which you wish to move armies for fortification." << endl;
		cin >> countryId;

		if (!cin.good()) {
			cout << "The id should be an integer." << endl;
			continue;
		}

		try {
			selectedCountry = player->mapPtr->getCountryById(countryId);

			if (isValidFortifiySource(destination, selectedCountry)) {
				validCountryId = true;
			}
			else {
				cout << "This Country is not a valid option." << endl;
			}
		}
		catch (exception e) {
			cout << "No countries have this id." << endl;
			continue;
		}
	} while (!validCountryId);

	return selectedCountry;
}

// Asks user for how many armies they want to move from source into destination
int PlayerStrategy::selectArmiesToMoveForFortification(Country * source, Country * destination)
{
	// The number of armies to move should be less than the number of armies on the source Country
	int selectedNumArmies;
	int max = source->armies;
	int min = 0;
	bool validNumArmies = false;

	do {
		cout << endl << "Please enter the number of armies you wish to move from country " << source->name;
		cout << " to country " << destination->name << " [" << min << "-" << (max - 1) << "]." << endl;
		cin >> selectedNumArmies;

		if (!cin.good()) {
			cout << "The number of armies to move should be an integer." << endl;
			continue;
		}

		if (selectedNumArmies < min) {
			cout << "You should move at least " << min << " armies." << endl;
			continue;
		}

		if (selectedNumArmies > max) {
			cout << "You cannot move more than " << max << " armies." << endl;
			continue;
		}

		validNumArmies = true;
	} while (!validNumArmies);

	return selectedNumArmies;
}

void PlayerStrategy::cheaterFortify()
{
	//do nothing
}

/*	Checks if 3 cards are valid for exchange.
	@return if have cards to exchange, return exchange armies; else return 0.
*/
int PlayerStrategy::exchangeAnyCardsForArmies() {

	// indexes of cards to exchange
	vector<int> cardsByType[4]; // temp container organized by 4 card types
	vector<int> cardsToExchange;
	int handCount = player->hand->getHandCount();

	for(int i = 0; i < handCount; i++) {
		switch(player->hand->playerHand->at(i).type)	
		{	// separate each card in hand by their type
			case INFANTRY:	cardsByType[0].push_back(i); break;
			case ARTILLERY:	cardsByType[1].push_back(i); break;
			case CAVALRY:	cardsByType[2].push_back(i); break;
			case WILD:		cardsByType[3].push_back(i); break;
		}

		// if found 3 of one type, break loop early
		// use assign to make deep copies
		if(cardsByType[0].size() == 3) { cardsToExchange.assign(cardsByType[0].begin(), cardsByType[0].end()); break; }
		if(cardsByType[1].size() == 3) { cardsToExchange.assign(cardsByType[1].begin(), cardsByType[1].end()); break; }
		if(cardsByType[2].size() == 3) { cardsToExchange.assign(cardsByType[2].begin(), cardsByType[2].end()); break; }
		// if found 1 of each type, break loop early
		if(cardsByType[0].size() > 0 && cardsByType[1].size() > 0 && cardsByType[2].size() > 0) { 
			cardsToExchange.push_back(cardsByType[0].at(0)); 
			cardsToExchange.push_back(cardsByType[1].at(0)); 
			cardsToExchange.push_back(cardsByType[2].at(0)); 
			break; 
		}
		// if found a wild card, choose any other 2 random cards and break loop early
		if(cardsByType[3].size() > 0) {
			// keep generating random index until 2 indexes points to a card that ISNT this wild card
			int rndIndex1 = -1, rndIndex2 = -1;
			while(rndIndex1 == rndIndex2 || rndIndex1 == i || rndIndex2 == i) {	
				rndIndex1 = genRandomNum(0, handCount - 1);
				rndIndex2 = genRandomNum(0, handCount - 1);
			}
			cardsToExchange.push_back(i); 
			cardsToExchange.push_back(rndIndex1); 
			cardsToExchange.push_back(rndIndex2); 
			break; 
		}
	}

	if(cardsToExchange.size() != 3)	{// if no sets of exchangable cards at all
		phaseLog->printMsg("0");
		return 0;
	}
	else {
		/// convert vector to array so it can pass to func param
		int exchangeCardsArr[3];
		for(int i = 0; i < 3; i++)
			exchangeCardsArr[i] = cardsToExchange.at(i);
		/// any elements with an index higher than the removed element's gets their index shifted by one (minus one).
		/// sort index with descending order so index doesnt shift when removing Card object from playerHand vector
		sort(begin(exchangeCardsArr), end(exchangeCardsArr), greater<int>());
		phaseLog->printMsg("Exchanging with card " + to_string(exchangeCardsArr[2] + 1) + 
										", card " + to_string(exchangeCardsArr[1] + 1) + 
										", and card " + to_string(exchangeCardsArr[0] + 1));
		tradeInCards(exchangeCardsArr);
		/// exchange is successful
		return player->hand->deck->getExchangedArmies();
	}
}

// generates a random number in between range
int PlayerStrategy::genRandomNum(int low, int high) 
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(low, high);
	return dist(mt);
}
