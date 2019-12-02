#include "Player.h"

#include <iostream>
#include <ctime>
#include <map>
using namespace std;

int* Player::currentGenId = new int(1);

//constructors
Player::Player() : id(new int(*currentGenId)), name(new string("Player " + to_string(*id))), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int, int>), hand(NULL), dice(new Dice()) {	genNextID(); }
Player::Player(string name, Deck * deck, Map * mapPtr, PlayerStrategy * aStrategy, PhaseLog* aPhaseLog) : id(new int(*currentGenId)), name(new string(name)), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int, int>), mapPtr(mapPtr), hand(new Hand(deck, ownedCountries, aStrategy)), dice(new Dice()), strategy(aStrategy), phaseLogPtr(aPhaseLog) { 
	genNextID();
}
// Copy constructor. Creates a new player with same fields (deep copied) with different id
Player::Player(const Player &p) {
	id = new int(*currentGenId);
	genNextID(); 
	name = new string(*p.name);
	ownedCountries = new map<int,Country*>(*p.ownedCountries);
	mapPtr = p.mapPtr;
	hand = new Hand(*p.hand);
	dice = new Dice(*p.dice);
	numOfOwnedCountriesPerContinent = new map<int,int>(*p.numOfOwnedCountriesPerContinent);
	strategy = p.strategy;
} 

// Destructor
Player::~Player() { delete id, name, ownedCountries, numOfOwnedCountriesPerContinent, hand, dice; }

// Assignment operator. Creates the same player with same id and fields.
const Player & Player::operator=(const Player & rightSide)
{
	if (&rightSide != this) {
		delete id;
		delete name;
		delete ownedCountries;
		delete numOfOwnedCountriesPerContinent;
		id = new int(*rightSide.id);
		name = new string(*rightSide.name);
		ownedCountries = new map<int, Country*>(*rightSide.ownedCountries);
		numOfOwnedCountriesPerContinent = new map<int, int>(*rightSide.numOfOwnedCountriesPerContinent);
		mapPtr = rightSide.mapPtr;
		strategy = rightSide.strategy;
		if (hand != rightSide.hand) {
			delete hand;
			hand = new Hand(*rightSide.hand);
		}
		if (dice!= rightSide.dice) {
			delete dice;
			dice = new Dice(*rightSide.dice);
		}
	}
	return *this;
}

/*	 Used in the startup phase of the game. Stores all owned countries and places 1 army on each.
	 And sets how many owned countries are in each continent for reinforcing armies.
*/
void Player::setOwnedCountries(list<Country*> countriesList) 
{
	for (Country* country : countriesList) {
		country->player = this;
		country->armies = 1;
		(*ownedCountries)[country->id] = country;
		(*numOfOwnedCountriesPerContinent)[country->continentId] += 1;
	}
}

//used during attack(). Adds this country to the ones owned by the player, places on it this many armies.
void Player::claimCountry(Country* country, int armies)
{
	country->player = this;
	country->armies = armies;
	(*ownedCountries)[country->id] = country;
	(*numOfOwnedCountriesPerContinent)[country->continentId] += 1;
	Notify();
}

//used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned.
Country* Player::loseCountry(int id)
{
	if (ownedCountries->count(id) == 0)
		return nullptr;
	else {
		Country* country = (*ownedCountries)[id];
		ownedCountries->erase(id);
		country->player = nullptr;
		country->armies = -1;
		(*numOfOwnedCountriesPerContinent)[country->continentId] -= 1;

		return country;
	}
}

/*	Get reinforcement armies and let player distribute given armies.
*/
void Player::reinforce() {

	phaseLogPtr->printMsg("\n" + *name + "'s turn!");
	phaseLogPtr->printMsg("---------------------------");
	phaseLogPtr->printMsg("//  Reinforcement Phase  //");
	int armies = 0, r = 0;
	armies += getCountryReinforcement();
	armies += getContinentReinforcement();

	while (hand->getHandCount() >= 3) {
		displayOwnedCountries();
		r += hand->exchange();
		if (r == 0)	// note 0 means user cancelled exchange action
			break;
		else if (hand->getHandCount() >= 3) 
			phaseLogPtr->printMsg("\nYou still have " + to_string(hand->getHandCount()) + " cards left.");
		
		armies += r;
		r = 0;
	}

	strategy->distributeArmies(armies);
}

//the player carries out a number of attacks
void Player::attack()
{
	phaseLogPtr->printMsg("\n/////////////////////////////");
	phaseLogPtr->printMsg("PLAYER " + *name + " ATTACK PHASE");
	phaseLogPtr->printMsg("/////////////////////////////");

	strategy->attackInit();
	bool atLeastOneCountryConquered = false;
	while (!strategy->isCheater() && strategy->decideToAttack()) {
		phaseLogPtr->printMsg("\nPlayer " + *name + " chose to attack!");

		// Select attacking and defending Countries
		Country* attackingCountry = strategy->selectAttackingCountry();
		Country* defendingCountry = strategy->selectDefendingCountry(attackingCountry);

		phaseLogPtr->printMsg("\nPlayer " + *name + " will use Country (" + to_string(attackingCountry->id) + ") " + attackingCountry->name + " to attack Player " + *(defendingCountry->player->name) + "'s Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name);

		// Select number of dice to roll
		int numAttackDice = strategy->selectNumAttackDice(attackingCountry);
		int numDefenseDice = strategy->selectNumDefenseDice(defendingCountry);

		phaseLogPtr->printMsg("\nPlayer " + *name + " will roll " + to_string(numAttackDice) + " dice");
		phaseLogPtr->printMsg("Player " + *(defendingCountry->player->name) + " will roll " + to_string(numDefenseDice) + " dice");

		// Roll dice
		list<int> attackRolls = dice->roll(numAttackDice);
		list<int> defenseRolls = dice->roll(numDefenseDice);

		string diceRolls = "\nPlayer " + *name + " rolled:";
		for (auto const roll : attackRolls) {
			diceRolls = diceRolls + " " + to_string(roll) + " ";
		}
		phaseLogPtr->printMsg(diceRolls);

		diceRolls = "Player " + *(defendingCountry->player->name) + " rolled:";
		for (auto const roll : defenseRolls) {
			diceRolls = diceRolls + " " + to_string(roll) + " ";
		}
		phaseLogPtr->printMsg(diceRolls);

		// Calculate army loss for attacking and defending Countries
		int attackerLoss = 0;
		int defenderLoss = 0;
		auto itAttackRolls = attackRolls.begin();
		auto itDefenseRolls = defenseRolls.begin();
		while (itAttackRolls != attackRolls.end() && itDefenseRolls != defenseRolls.end()) {
			if (*itAttackRolls > *itDefenseRolls) {
				defenderLoss++;
			}
			else {
				attackerLoss++;
			}

			itAttackRolls++;
			itDefenseRolls++;
		}

		phaseLogPtr->printMsg("\nPlayer " + *name + " loses " + to_string(attackerLoss) + " armies in Country (" + to_string(attackingCountry->id) + ") " + attackingCountry->name);
		phaseLogPtr->printMsg("Player " + *(defendingCountry->player->name) + " loses " + to_string(defenderLoss) + " armies in Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name);

		// Remove army loss from armies in attacking Country
		addOrRemoveArmies(attackingCountry->id, -1 * attackerLoss);
		phaseLogPtr->printMsg("\nThere are now " + to_string(attackingCountry->armies) + " armies in Country (" + to_string(attackingCountry->id) + ") " + attackingCountry->name);

		// If all armies in defending Country are defeated, attacking Player conquers the defending Country
		if (defendingCountry->armies - defenderLoss <= 0) {
			phaseLogPtr->printMsg("\nPlayer " + *name + " defeated all of Player " + *(defendingCountry->player->name) + "'s armies in Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name + "!");
			phaseLogPtr->printMsg("Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name + " now belongs to Player " + *name);

			// Calculate how many armies to move in newly conquered Country
			int numArmiesToMove = strategy->selectNumArmiesToMoveAfterAttackSuccess(attackingCountry, defendingCountry, numAttackDice);

			phaseLogPtr->printMsg("Player " + *name + " will move " + to_string(numArmiesToMove) + " armies in Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name);

			// Change the defending Country's ownership and move armies in it
			defendingCountry->player->loseCountry(defendingCountry->id);
			claimCountry(defendingCountry, numArmiesToMove);
			addOrRemoveArmies(attackingCountry->id, -1 * numArmiesToMove);

			phaseLogPtr->printMsg("\nThere are now " + to_string(attackingCountry->armies) + " armies in Country (" + to_string(attackingCountry->id) + ") " + attackingCountry->name + "\n");

			atLeastOneCountryConquered = true;
		}
		else {
			// Remove army loss from defending Country's armies
			defendingCountry->player->addOrRemoveArmies(defendingCountry->id, -1 * defenderLoss);
			phaseLogPtr->printMsg("\nThere are now " + to_string(defendingCountry->armies) + " armies in Country (" + to_string(defendingCountry->id) + ") " + defendingCountry->name + "\n");
		}
	}

	if (!strategy->isCheater()) {
		phaseLogPtr->printMsg("\nPlayer " + *name + " chose not to attack\n");
	} else {
		strategy->cheaterAttack();
	}

	if (atLeastOneCountryConquered) {
		//TODO in A3: check if Player defeated other Player. Transfer cards and impose trading if necessary.
		/* PSEUDOCODE
		if(player defeats another player/or players) {
			- transfer cards here

			// trigger mandatory exchange after claiming enemy's cards
			if(hand->getHandCount() >= 6) {
				while(hand->getHandCount() > 4)
					exchange();	// force exchange until have 4 cards or less
			}
		}
		*/
			
		hand->drawFromDeck();
	}

	phaseLogPtr->printMsg("\n/////////////////////////////");
	phaseLogPtr->printMsg("END OF PLAYER " + *name + " ATTACK PHASE");
	phaseLogPtr->printMsg("/////////////////////////////");
}

//move a number of armies from an owned country to another owned neighboring country
void Player::fortify()
{
	phaseLogPtr->printMsg("\n/////////////////////////////");
	phaseLogPtr->printMsg("PLAYER " + *name + " FORTIFICATION PHASE");
	phaseLogPtr->printMsg("/////////////////////////////");

	Country* source = nullptr;
	Country* destination = nullptr;
	int armiesToMove = 0;

	if (!strategy->isCheater()) {
		// Choose whether to fortify or not
		if (!strategy->decideToFortify()) {
			phaseLogPtr->printMsg("\nPlayer " + *name + " chose not to fortify");
			return;
		}

		// Choose a the Country that will be fortified (destination)
		destination = strategy->selectFortifyDestination();
		phaseLogPtr->printMsg("\nPlayer " + *name + " chose to fortify Country (" + to_string(destination->id) + ") " + destination->name);

		// Choose the Country from which armies will be moved into destination (source)
		source = strategy->selectFortifySource(destination);
		phaseLogPtr->printMsg("Player " + *name + " chose to use armies from Country (" + to_string(source->id) + ") " + source->name + " to fortify Country (" + to_string(destination->id) + ") " + destination->name);

		// Choose the number of armies to move from source into destination
		armiesToMove = strategy->selectArmiesToMoveForFortification(source, destination);
		phaseLogPtr->printMsg("Player " + *name + " chose to move " + to_string(armiesToMove) + " armies from Country (" + to_string(source->id) + ") " + source->name + " into Country (" + to_string(destination->id) + ") " + destination->name);
	} else {
		strategy->cheaterFortify();
	}
	
	try {
		if (!strategy->isCheater()) {
			// Remove armies from source and add them in destination
			addOrRemoveArmies(source->id, -armiesToMove);
			addOrRemoveArmies(destination->id, armiesToMove);
		}
		phaseLogPtr->printMsg("\n/////////////////////////////");
		phaseLogPtr->printMsg("END OF PLAYER " + *name + " FORTIFICATION PHASE");
		phaseLogPtr->printMsg("/////////////////////////////");
	}
	catch (invalid_argument e) {
		cout << "Exception thrown when trying to add/remove armies:" << endl;
		cout << e.what() << endl;
	}
}

/*	Get armies from total countries divided by 3, removing fractions.
	@return army reinforcement from owned country count
*/
int Player::getCountryReinforcement() {

	int reinforcements = (ownedCountries->size() / 3);
	if (reinforcements < 3)
		reinforcements = 3;	// 3 being minimum
	return reinforcements;
}

/*	Get armies from owning any continent.
	@return army reinforcement from owned continent
*/
int Player::getContinentReinforcement() {
	
	int reinforcements = 0;
	list<Continent> allContinents = mapPtr->getContinents();

	for(Continent c : allContinents) {
		if(c.getSize() == (*numOfOwnedCountriesPerContinent)[c.getId()])
				{ reinforcements += c.getWorth(); }
	}
	return reinforcements;
}

/*	Armies can be a +ve or -ve integer, meaning add or remove this many armies.
	@exception throws if country is not owned or if the number of armies to remove >= current number of armies.
*/
void Player::addOrRemoveArmies(int countryId, int armies)
{
	if (ownedCountries->count(countryId) == 0)
		throw invalid_argument("Could not add/remove armies because the country with id " + to_string(countryId) + " is not owned by Player " + to_string(*id));
	else {
		int currentArmies = (*ownedCountries)[countryId]->armies;
		if (currentArmies + armies < 1)
			throw invalid_argument("Could not remove " + to_string(abs(armies)) + " armies from country with id " + to_string(countryId) + " because it currently has " + to_string(currentArmies) + " armies.");
		else
			(*ownedCountries)[countryId]->armies = currentArmies + armies;
	}
}

/*	Display owned countries' armies and continent id.
*/
void Player::displayOwnedCountries() {

	phaseLogPtr->printMsg("\nOwned Countries   Armies  Continent");
	phaseLogPtr->printMsg("====================================");
	map<int, Country*>::iterator iter = ownedCountries->begin();
	for (; iter != ownedCountries->end(); ++iter) {
		phaseLogPtr->printMsg("Country " + to_string(iter->first) + 
								" \t|| " + to_string(iter->second->armies) +
								 "\t|| " + to_string(iter->second->continentId));
	}
}

/////////////////////////
/* PHASE LOG */
/////////////////////////

PhaseLog::PhaseLog() : msg(new string("")) {}
PhaseLog::~PhaseLog() { delete msg; };
PhaseLog::PhaseLog(const PhaseLog &pl) : msg(new string(*pl.msg)) {}
PhaseLog& PhaseLog::operator=(const PhaseLog &pl) {
	if(&pl != this) {
		delete msg;
		msg = new string(*pl.msg);
	}
	return *this;
}

void PhaseLog::printMsg(const string &s) {
	delete msg;
	msg = new string(s);
	Notify();
}