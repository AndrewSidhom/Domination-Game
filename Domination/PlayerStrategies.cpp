#include "PlayerStrategies.h"

#include <iostream>

using std::list;
using std::cout;
using std::cin;

/* AGGRESSIVE PLAYER STRATEGY */

// Find the strongest Country owned by the Player (Country with most armies) that can attack other Countries
void AgressivePlayerStrategy::attackInit()
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
bool AgressivePlayerStrategy::decideToAttack()
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

Country * AgressivePlayerStrategy::selectAttackingCountry()
{
	return strongestAttackingCountry;
}

Country * AgressivePlayerStrategy::selectDefendingCountry(Country * attackingCountry)
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
int AgressivePlayerStrategy::selectNumAttackDice(Country * attackingCountry)
{
	// The attack is allowed min: 1 die, max: (number of armies in the attacking Country - 1) dice
	// Will select the max number of dice to roll
	int max = attackingCountry->armies - 1;

	return max;
}

// Returns an integer - the number of dice to be rolled by the defender
int AgressivePlayerStrategy::selectNumDefenseDice(Country * defendingCountry)
{
	// The defense is allowed min: 1 die, max: 2 dice if the number of armies in defendingCountry >= 2
	// Will select the min number of dice to roll

	return 1;
}

// Returns the number of armies the attacker will move in newly conquered country
int AgressivePlayerStrategy::selectNumArmiesToMoveAfterAttackSuccess(Country * attackingCountry, Country * defendingCountry, int diceRolled)
{
	// The Player is allowed to move min: (number of dice rolled) armies, max: (number of armies in attacking Country - 1) armies
	// Will select the max number
	int max = attackingCountry->armies - 1;

	return max;
}

// Returns the strongest Country which can be fortified. Returns a nullptr if no Country owned by the Player can be fortified
Country * AgressivePlayerStrategy::selectFortifyDestination()
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
Country * AgressivePlayerStrategy::selectFortifySource(Country * destination)
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
int AgressivePlayerStrategy::selectArmiesToMoveForFortification(Country * source, Country * destination)
{
	// The number of armies to move should be less than the number of armies on the source Country
	// Will choose the max number of armies that can be moved (armies on source - 1)
	return source->armies - 1;
}


/* BENEVOLENT PLAYER STRATEGY */

// Returns false. A benevolent Player never attacks.
bool BenevolentPlayerStrategy::decideToAttack()
{
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

	return nullptr;
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

	return nullptr;
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


/* DEFAULT (HUMAN) PLAYER STRATEGY */

void PlayerStrategy::attackInit()
{
	// do nothing
	return;
}

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

	// Add print of countries owned by Player + enemy neighbours
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

Country * PlayerStrategy::selectAttackingCountry()
{
	// display country info again

	int countryId;
	Country* selectedCountry;
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

Country * PlayerStrategy::selectDefendingCountry(Country * attackingCountry)
{
	// display info on neighbors of attackingCountry
	
	int countryId;
	Country* selectedCountry;
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
