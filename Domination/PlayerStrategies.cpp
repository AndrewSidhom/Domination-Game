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

/* AGGRESSIVE PLAYER STRATEGY */

AggressivePlayerStrategy::AggressivePlayerStrategy() : strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* aPlayer) : PlayerStrategy(aPlayer), strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy & strategy) : PlayerStrategy(strategy.player), strongestAttackingCountry(nullptr) {}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

const AggressivePlayerStrategy & AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy & rightSide)
{
	if (&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;

// Returns true when have cards >= 3
bool AggressivePlayerStrategy::ifPlayerWantsToExchange() 
{
	return (player->hand->getHandCount() >= 3);	// attempt exchange whenever it's possible
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
	cout << "\nCountries you own matches with your exchanged cards:\n";
		for (int id : *matchingCountries)
			cout << "Country " << id << "|" << endl;

	// choose strongest country
	int strongestCountryId = matchingCountries->at(0);
	int highestArmyCount = player->hand->ownedCountries->at(matchingCountries->at(0))->armies;
	for(int id : *matchingCountries) {
		if(player->hand->ownedCountries->at(id)->armies > highestArmyCount)
			strongestCountryId = id;
	}

	player->hand->ownedCountries->at(strongestCountryId)->armies += 2;
	cout << "Choose a country to give 2 additional armies: " << strongestCountryId;
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

Country * AggressivePlayerStrategy::selectAttackingCountry()
{
	return strongestAttackingCountry;
}

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


/* BENEVOLENT PLAYER STRATEGY */

BenevolentPlayerStrategy::BenevolentPlayerStrategy() {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* aPlayer) : PlayerStrategy(aPlayer) {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy & strategy) : PlayerStrategy(strategy.player) {}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

const BenevolentPlayerStrategy & BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy & rightSide)
{
	if (&rightSide != this) {
		PlayerStrategy::operator=(rightSide);
	}
	return *this;

// Returns true when have cards >= 3
bool BenevolentPlayerStrategy::ifPlayerWantsToExchange() 
{
	return (player->hand->getHandCount() >= 3);	// attempt exchange whenever it's possible
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
	cout << "\nCountries you own matches with your exchanged cards:\n";
		for (int id : *matchingCountries)
			cout << "Country " << id << "|" << endl;

	// choose weakest country
	int weakestCountryId = matchingCountries->at(0);
	int lowestArmyCount = player->hand->ownedCountries->at(matchingCountries->at(0))->armies;
	for(int id : *matchingCountries) {
		if(player->hand->ownedCountries->at(id)->armies < lowestArmyCount)
			weakestCountryId = id;
	}

	player->hand->ownedCountries->at(weakestCountryId)->armies += 2;
	cout << "Choose a country to give 2 additional armies: " << weakestCountryId;
}

// Returns false. A benevolent Player never attacks.
bool BenevolentPlayerStrategy::decideToAttack()
{
	return false;
}

// Returns true if the Player can fortify (the Benevolent Player fortifies by default if they can fortify)
bool BenevolentPlayerStrategy::decideToFortify()
{
	return canFortify();
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

PlayerStrategy::PlayerStrategy() : player(nullptr) {}
PlayerStrategy::PlayerStrategy(Player* aPlayer) : player(aPlayer) {}
PlayerStrategy::PlayerStrategy(const PlayerStrategy & strategy) : player(strategy.player) {}

PlayerStrategy::~PlayerStrategy() {}

const PlayerStrategy& PlayerStrategy::operator =(const PlayerStrategy& rightSide) {
	if (&rightSide != this) {
		player = rightSide.player;
		player->hand = rightSide.player->hand;
	}
	return *this;

//	Return if player wants to exchange
bool PlayerStrategy::ifPlayerWantsToExchange() 
{
	string input;
	do {
		cout << "\nWould you like to exchange your cards? (y/n): " << endl;
		cin >> input;

		if(input.compare("y") == 0 || input.compare("n") == 0)  // 0 means equal
			break;
		else
			cout << "\nInput must be 'y' or 'n'\n";
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
	int* cardsToExchangeIndex = new int[3];

	do {
		/// get player input
		int selectedCardIndex = getPlayersCardOfChoice(isMandatory, numOfCardsChosen, cardsToExchangeIndex);

		if (selectedCardIndex == 0 && !isMandatory) {	/// if player wants to cancel
			cout << "\nExchange action cancelled.\n";
			return 0;
		}
		else {	/// store chosen card
			cardsToExchangeIndex[numOfCardsChosen] = selectedCardIndex - 1;
			numOfCardsChosen++;
		}

		if (numOfCardsChosen == 3) {
			if (player->hand->isValidExchangeCards(cardsToExchangeIndex[0], cardsToExchangeIndex[1], cardsToExchangeIndex[2]))
			{
				tradeInCards(cardsToExchangeIndex);
				delete [] cardsToExchangeIndex;
				/// exchange is successful
				return player->hand->deck->getExchangedArmies();
			}
			else {
				cout << "\nCannot exchange with these cards. Must be a matching or of consecutive types.\n";
				numOfCardsChosen = 0;
			}
		}
	} while (numOfCardsChosen != 3);
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
			cout << "\nInvalid number input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else if (selectedCardIndex == 0 && isMandatory)
			cout << "\nYou've reached the card limit and must exchange.\n";
		else if (selectedCardIndex < 0 || selectedCardIndex > player->hand->playerHand->size())
			cout << "\nYour choice must be within your hand's cards.\n";
		else if (selectedCardIndex == cardsToExchangeIndex[0]+1 || 
				selectedCardIndex == cardsToExchangeIndex[1]+1)
			cout << "\nYou have already selected this card.\n";
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
void PlayerStrategy::tradeInCards(int* cardsToExchange[]) 
{
	/// give bonus +2 armies if cards match owned countries
	vector<int>* matchingCountries = new vector<int>();
	
	for (int i = 0; i < 3; i++) {	/// loop each exchanged cards if matches owned countries	
		int countryId = player->hand->playerHand->at(*cardsToExchange[i]).countryId;
		if(player->hand->ownedCountries->count(countryId) > 0)	/// 0 if not an element, 1 if is
			{ matchingCountries->push_back(countryId);	}	/// store country by reference
	}
	/// if have matching, prompt player input to choose which country to give +2 units
	if (!matchingCountries->empty()) 
		distributeExchangeBonus(matchingCountries);
	delete matchingCountries;

	/// remove exchanged cards from hand
	/// any elements with an index higher than the removed element's gets their index shifted by one (minus one).
	/// sort index with descending order so index doesnt shift when removing Card object from playerHand vector
	sort(begin(*cardsToExchangeIndex), end(*cardsToExchangeIndex), greater<int>());
	player->hand->playerHand->erase(hand->playerHand->begin() + *cardsToExchangeIndex[0]);
	player->hand->playerHand->erase(hand->playerHand->begin() + *cardsToExchangeIndex[1]);
	player->hand->playerHand->erase(hand->playerHand->begin() + *cardsToExchangeIndex[2]);
}

/*	Prompt user to choose which country that matches the exchanged cards to receive +2 bonus armies.
	@param country ids that matches exchanged cards
*/
void PlayerStrategy::distributeExchangeBonus(vector<int>* matchingCountries) 
{
	cout << "\nCountries you own matches with your exchanged cards:\n";
		for (int id : *matchingCountries)
			cout << "Country " << id << "|" << endl;

	int selectedCountryId;
	do {
		cout << "Choose a country to give 2 additional armies: ";
		cin >> selectedCountryId;
		
		if (!cin.good()) /// !good() when input isnt integer
		{
			cout << "\nInvalid number input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else {
			for(int id : *matchingCountries) {
				if(selectedCountryId == id)
					break;
			}
			cout << "\nPlease choose the countries given.\n";
		}
	} 
	while (true);

	player->hand->ownedCountries->at(selectedCountryId)->armies += 2;
}

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

bool PlayerStrategy::decideToFortify()
{
	if (!canFortify()) {
		cout << endl << "None of your countries can be fortified." << endl;
		cout << "Your fortification phase will be skipped." << endl;

		return false;
	}

	// Add print of countries owned by Player or something
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

int PlayerStrategy::selectArmiesToMoveForFortification(Country * source, Country * destination)
{
	// The number of armies to move should be less than the number of armies on the source Country
	int selectedNumArmies;
	int max = source->armies;
	int min = 0;
	bool validNumArmies = false;

	do {
		cout << endl << "Please enter the number of armies you wish to move from country " << source->name;
		cout << " to country " << destination->name << " [" << min << "-" << max << "]." << endl;
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

/*	Checks if 3 cards are valid for exchange.
	@return if have cards to exchange, return exchange armies; else return 0.
*/
int PlayerStrategy::exchangeAnyCardsForArmies() {

	// indexes of cards to exchange
	int cardIndexes[4][3]; // 4 card types, max 3 size (3 of 1 type => matching => exchange immediately)
	int infCtr = 0, artCtr = 0, cavCtr = 0, wildCtr = 0;
	int* cardsToExchange = nullptr;
	int handCount = player->hand->getHandCount();

	for(int i = 0; i < handCount; i++) {
		switch(player->hand->playerHand->at(i).type)	
		{	// separate each card in hand by their type
			case INFANTRY:	cardIndexes[0][infCtr] = i; infCtr++; break;
			case ARTILLERY:	cardIndexes[1][artCtr] = i; artCtr++; break;
			case CAVALRY:	cardIndexes[2][cavCtr] = i; cavCtr++; break;
			case WILD:		cardIndexes[3][wildCtr] = i; wildCtr++; break;
		}

		// if found 3 of one type, break loop early
		if(infCtr == 3)	{ cardsToExchange = cardIndexes[0]; break; }
		if(artCtr == 3)	{ cardsToExchange = cardIndexes[1]; break; }
		if(cavCtr == 3)	{ cardsToExchange = cardIndexes[2]; break; }
		// if found 1 of each type, break loop early
		if(infCtr > 0 && artCtr > 0 && cavCtr > 0) { 
			cardsToExchange = new int[3];
			cardsToExchange[0] = cardIndexes[0][0]; 
			cardsToExchange[1] = cardIndexes[1][0]; 
			cardsToExchange[2] = cardIndexes[2][0]; 
			break; 
		}
		// if found a wild card, choose any other 2 random cards and break loop early
		if(wildCtr > 0) {
			// keep generating random index until 2 indexes points to a card that ISNT this wild card
			int rndIndex1 = -1, rndIndex2 = -1;
			while(rndIndex1 == rndIndex2 || rndIndex1 == i || rndIndex2 == i) {	
				rndIndex1 = genRandomNum(0, handCount - 1);
				rndIndex2 = genRandomNum(0, handCount - 1);
			}
			cardsToExchange = new int[3];
			cardsToExchange[0] = i; 
			cardsToExchange[1] = rndIndex1; 
			cardsToExchange[2] = rndIndex2; 
			break; 
		}
	}

	if(cardsToExchange == nullptr)	// if no sets of exchangable cards at all
		return 0;
	else {
		cout << "Exchanging with card " << cardsToExchange[0] << ", card " << cardsToExchange[1] << ", and card " << cardsToExchange[2];
		tradeInCards(cardsToExchange);
		delete [] cardsToExchange;
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
