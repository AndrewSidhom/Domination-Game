#include "Player.h"

#include <iostream>
#include <ctime>
using namespace std;

int* Player::currentGenId = new int(1);

//constructor, destructor
Player::Player() : id(new int(*currentGenId)), name(new string("Player " + to_string(*id))), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int, int>), hand(NULL), dice(new Dice()) {	genNextID(); }
Player::Player(string name, Deck * deck, Map * mapPtr, PlayerStrategy * aStrategy, PhaseLog* aPhaseLog) : id(new int(*currentGenId)), name(new string(name)), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int, int>), mapPtr(mapPtr), hand(new Hand(deck, ownedCountries, aStrategy)), dice(new Dice()), strategy(aStrategy), phaseLogPtr(aPhaseLog) { 
	genNextID();
	aStrategy->setPlayer(this);
}
Player::Player(const Player &p) : id(p.id), name(p.name), ownedCountries(p.ownedCountries), mapPtr(p.mapPtr), hand(p.hand), dice(p.dice), numOfOwnedCountriesPerContinent(p.numOfOwnedCountriesPerContinent), strategy(p.strategy) {}
Player::~Player() { delete id, name, ownedCountries, numOfOwnedCountriesPerContinent, hand, dice, mapPtr, strategy; }

const Player & Player::operator=(const Player & rightSide)
{
	if (&rightSide != this) {
		delete id;
		delete name;
		delete ownedCountries;
		delete numOfOwnedCountriesPerContinent;
		delete hand;
		delete dice;

		id = new int(*rightSide.id);
		name = new string(*rightSide.name);
		*ownedCountries = *rightSide.ownedCountries;
		*numOfOwnedCountriesPerContinent = *numOfOwnedCountriesPerContinent;
		mapPtr = rightSide.mapPtr;
		hand = new Hand(*rightSide.hand);
		dice = new Dice(*rightSide.dice);
		strategy = rightSide.strategy;
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

// TODO: to be removed?
//rolls this number of dice, returns dice results
vector<int> Player::rollDice(int howMany)
{
	//needs the Dice class to provide a roll() function that takes the number of dice as parameter. 
	return vector<int>();
}

/*	Get reinforcement armies and let player distribute given armies.
*/
void Player::reinforce() {

	cout << "**Reinforcement Phase**\n";
	int armies = 0, r = 0;
	armies += getCountryReinforcement();
	armies += getContinentReinforcement();

	while (hand->getHandCount() >= 3) {
		r += hand->exchange();
		if (r == 0)	// note 0 means user cancelled exchange action
			break;
		else if (hand->getHandCount() >= 3)
			cout << "\nYou still have " << hand->getHandCount() << " cards left.\n";
		armies += r;
		r = 0;
	}

	distributeArmies(armies);
}

//the player carries out a number of attacks
void Player::attack()
{
	phaseLogPtr->printMsg("\n/////////////////////////////");
	phaseLogPtr->printMsg("PLAYER " + *name + " ATTACK PHASE");
	phaseLogPtr->printMsg("/////////////////////////////");

	strategy->attackInit();
	bool atLeastOneCountryConquered = false;

	while (strategy->decideToAttack()) {
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

	phaseLogPtr->printMsg("\nPlayer " + *name + " chose not to attack\n");

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

	if (!strategy->decideToFortify()) {
		phaseLogPtr->printMsg("\nPlayer " + *name + " chose not to fortify");
		return;
	}

	Country* destination = strategy->selectFortifyDestination();
	phaseLogPtr->printMsg("\nPlayer " + *name + " chose to fortify Country (" + to_string(destination->id) + ") " + destination->name);

	Country* source = strategy->selectFortifySource(destination);
	phaseLogPtr->printMsg("Player " + *name + " chose to use armies from Country (" + to_string(source->id) + ") " + source->name + " to fortify Country (" + to_string(destination->id) + ") " + destination->name);

	int armiesToMove = strategy->selectArmiesToMoveForFortification(source, destination);
	phaseLogPtr->printMsg("Player " + *name + " chose to move " + to_string(armiesToMove) + " armies from Country (" + to_string(source->id) + ") " + source->name + " into Country (" + to_string(destination->id) + ") " + destination->name);

	try {
		addOrRemoveArmies(source->id, -armiesToMove);
		addOrRemoveArmies(destination->id, armiesToMove);

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

/*	Prompt user to choose which countries to distribute their reinforcement armies
	@param int: total armies from reinforcement to distribute
*/
void Player::distributeArmies(int totalArmies) {

	int countryInput, armiesInput, i = 0;

	displayOwnedCountries();
	while(totalArmies > 0) 
	{
		cout << "---\nYou have " << totalArmies << " armies" << (i++? " left.\n" : ". Deploy your armies!\n"); // ++ to show different msg the second time
		try {
			// prompt choose country to add armies
			cout << "Country: ";
			countryInput = promptNumberInput();
			if(ownedCountries->count(countryInput) == 0)
				throw "\nYou do not own this country.";
			// prompt # of armies to place in that chosen country
			cout << "Armies: ";
			armiesInput = promptNumberInput();
			if (armiesInput > totalArmies || armiesInput < 0) 
				throw ("\nYou must enter armies in the range given.");
			
			addOrRemoveArmies(countryInput, armiesInput);
			totalArmies -= armiesInput;
		}
		catch(const char* msg) {
			cout << msg << endl;
		}
	}
	displayOwnedCountries();
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

/*	Prompt user to input a NUMBER only.
	@return valid number input
*/
int Player::promptNumberInput() {

	int n;
	do {
		cin >> n;
		if (!cin.good()) {	/// !good() when input doesnt match declared type
			cout << "\nInvalid number input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else break;
	} while (true);

	return n;
}

/*	Display owned countries' armies and continent id.
*/
void Player::displayOwnedCountries() {

	cout << "\nOwned Countries   Armies  Continent\n"
		<< "====================================";
	map<int, Country*>::iterator iter = ownedCountries->begin();
	for (; iter != ownedCountries->end(); ++iter)
	{
		cout << "\nCountry " << iter->first << " \t|| " << iter->second->armies << "\t|| " << iter->second->continentId;
	}
	cout << endl;
}

/////////////////////////
/* PHASE LOG */
/////////////////////////

PhaseLog::PhaseLog() : msg(new string("")) {}
PhaseLog::~PhaseLog() { delete msg; };
PhaseLog::PhaseLog(const PhaseLog &pl) : msg(pl.msg) {}
PhaseLog& PhaseLog::operator=(const PhaseLog &pl) {
	msg = pl.msg;
	return *this;
}

void PhaseLog::printMsg(const string &s) {
	delete msg;
	msg = new string(s);
	Notify();
}