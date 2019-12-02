#pragma once
#include "Map.h"
#include <vector>
using std::vector;

// forward-declaration to resolve circular dependency
class Player;
class PhaseLog;

// The Player Strategy to be inherited by Concrete Stragies. Its default behaviour is that of a HUMAN Player 
// (requests for input for any kind of decision)
class PlayerStrategy {
public:
	// Constants
	const int MIN_NUM_ATTACK_DICE = 1; // minimum number of dice required for attack
	const int MIN_NUM_DEFENSE_DICE = 1; // minimum number of dice required for defending from attack
	const int MAX_NUM_DEFENSE_DICE = 2; // maximum number of dice that can be used in defending from an attack
	const int MIN_NUM_ARMIES_ON_COUNTRY = 1; // minimum number of armies required to be in a Country

	// Constructors, destructors
	PlayerStrategy();
	PlayerStrategy(PhaseLog* phaseLogPtr);
	PlayerStrategy(const PlayerStrategy& strategy);
	virtual ~PlayerStrategy();

	// Assignment operator
	PlayerStrategy& operator =(const PlayerStrategy& rightSide);

	void setPlayer(Player* aPlayer) { player = aPlayer; }; // Set the player data member
	void setPhaseLog(PhaseLog* phaseLogPtr) { phaseLog = phaseLogPtr; };

	virtual bool isCheater() { return false; } // indicates whether a strategy is a cheater strategy

	// REINFORCE
	virtual bool ifPlayerWantsToExchange(); // Prompt if user wants to exchange
	virtual int promptExchangeForArmies(bool isMandatory); // Prompt user to choose which cards to exchange and get armies received
	virtual void distributeArmies(int totalArmies); // Prompt user to choose which countries to distribute their reinforcement armies

	// ATTACK
	virtual void attackInit(); // Do whatever initialization is required before deciding to attack or not
	virtual bool decideToAttack(); // Choose whether to attack an enemy country or not
	virtual Country* selectAttackingCountry(); // Choose which Country to attack from. This Country should have at least 2 armies within it and should have neighbouring enemy Countries
	virtual Country* selectDefendingCountry(Country* attackingCountry); // Choose a Country to attack from the attacking Country
	virtual int selectNumAttackDice(Country* attackingCountry); // Choose the number of dice to roll for attack
	virtual int selectNumDefenseDice(Country* defendingCountry); // Choose the number of dice to roll for defending against an attack
	virtual int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled); // Choose the number of armies to move from the attackingCountry to the defendingCountry after the Player won an attack
	virtual void cheaterAttack(); // attack method specifically for the cheater strategy

	// FORTIFY
	virtual bool canFortify(); // Whether a Player has a Country that can be fortified
	bool isValidFortifyDestination(Country* destination); // Returns true if a Country is a valid fortification destination
	bool isValidFortifiySource(Country* destination, Country* source); // Returns true if a Country is a valid fortification source for a given destination
	virtual bool decideToFortify(); // Choose whether to fortify a Country or not
	virtual Country* selectFortifyDestination(); // Choose which Country should be fortified
	virtual Country* selectFortifySource(Country* destination); // Choose a Country from which to move armies into the destination Country
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination); // Choose how many armies to move from the source Country into the destination Country
	virtual void cheaterFortify(); // fortify method specifically for the cheater strategy

protected:
	Player* player; // The current Player using this strategy
	PhaseLog* phaseLog; // Phase logger

	int exchangeAnyCardsForArmies(); // if have cards to exchange, return exchange armies; else return 0 (has no user input - mostly for AI)
	void tradeInCards(int cardsToExchange[]); // After choosing exchanged cards, trade in the cards
	virtual void distributeExchangeBonus(vector<int>* matchingCountries); //	Prompt user to choose which country that matches the exchanged cards to receive +2 bonus armies.
	virtual int promptCountryToReinforce(); // prompt which country to reinforce
	virtual int promptNumOfArmiesToPlace(int totalArmies); // prompt how many armies to place

	// RELATED SERVICE FUNCS
	int genRandomNum(int low, int high); // generates a random number in between range

private:
	// REINFORCE
	int getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen, int cardsToExchangeIndex[]); //	Prompt user to choose which card from their hand to exchange.
	// RELATED SERVICE FUNCS
	int promptNumberInput(); // Prompt user to input a NUMBER only.
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
	// Constructors, destructor
	AggressivePlayerStrategy();
	AggressivePlayerStrategy(PhaseLog* phaseLogPtr);
	AggressivePlayerStrategy(const AggressivePlayerStrategy& strategy);
	virtual ~AggressivePlayerStrategy();

	// Assignment operator
	AggressivePlayerStrategy& operator =(const AggressivePlayerStrategy& rightSide);

	// Reinforce methods
	// Strategy: reinforce strongest country
	bool ifPlayerWantsToExchange(); // Returns true when have cards >= 3
	int promptExchangeForArmies(bool isMandatory); // Automatically choose any cards in hands to exchange (has no user input)
	
	// Attack methods
	// Goal: Continue attacking until the strongest Country has no more Countries it can attack
	void attackInit(); // Find the strongest Country owned by the Player (Country with most armies) that can attack other Countries
	bool decideToAttack(); // Will attack if the strongest Country can still attack
	Country* selectAttackingCountry(); // Returns the strongest Country
	Country* selectDefendingCountry(Country* attackingCountry); // Returns a neighbour of the strongest Country that can be attacked
	int selectNumAttackDice(Country* attackingCountry); // Selects the max number of dice
	int selectNumDefenseDice(Country* defendingCountry); // Chooses the minimum number of dice
	int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled); // Chooses the max number of armies it can move

	// Fortify methods
	virtual bool decideToFortify(); // Will fortify if they can
	Country* selectFortifyDestination(); // Will pick the strongest Country that can be fortified
	Country* selectFortifySource(Country* destination); // Will pick the neighbor owned by the Player with the most armies
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination); // Will move the most armies it can

protected:
	// REINFORCE
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with most armies
	int promptCountryToReinforce(); // return any country with most armies
	int promptNumOfArmiesToPlace(int totalArmies); // return all of given armies

private:
	Country* strongestAttackingCountry; // The Country owned by the Player which has the most armies on it and can attack at least one other Country at the beginning of the Player's attack phase
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	// Constructors, destructor
	BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(PhaseLog* phaseLogPtr);
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& strategy);
	virtual ~BenevolentPlayerStrategy();
	// Assignment operator
	BenevolentPlayerStrategy& operator =(const BenevolentPlayerStrategy& rightSide);
	
	// Reinforce methods
	// Strategy: reinforce weakest countries
	bool ifPlayerWantsToExchange(); // Returns true when have cards >= 3
	int promptExchangeForArmies(bool isMandatory); // Automatically choose any cards in hands to exchange (has no user input)

	// Attack methods
	bool decideToAttack(); // Will never attack
	int selectNumDefenseDice(Country* defendingCountry); // Will return the max number of dice that can be rolled for defense

	// Fortify methods
	virtual bool decideToFortify(); // Will choose to fortify if there is a Country weaker than others owned by the Player that can be fortified
	Country* selectFortifyDestination(); // Will choose the weakest Country owned by the Player that can be fortified
	Country* selectFortifySource(Country* destination); // Will choose the strongest neigbor of destination owned by the Player
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination); // Will distribute armies such that the number of armies in source and destination is equal as much as possible

protected:
	// REINFORCE
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with least armies
	int promptCountryToReinforce(); // return any country with least armies
	int promptNumOfArmiesToPlace(int totalArmies); // return all of given armies
};

class RandomPlayerStrategy : public PlayerStrategy
{
public:
	// Constructors, destructor
	RandomPlayerStrategy();
	RandomPlayerStrategy(PhaseLog* phaseLogPtr);
	RandomPlayerStrategy(const RandomPlayerStrategy& strategy);
	virtual ~RandomPlayerStrategy();
	// Assignment operator
	RandomPlayerStrategy& operator =(const RandomPlayerStrategy& rightSide);

	// REINFORCE
	bool ifPlayerWantsToExchange(); // If have >= 3 cards, make a fifty-fifty decision to exchange or not.
	int promptExchangeForArmies(bool isMandatory); // Prompt user to choose which cards to exchange and get armies received

	// Attack methods
	// Goal: Attack at random
	bool decideToAttack(); // randomly decide to attack
	Country* selectAttackingCountry(); // randomly pick an attacking country
	Country* selectDefendingCountry(Country* attackingCountry); // randomly select a country to attack
	int selectNumAttackDice(Country* attackingCountry); // randomly select number of attack dice
	int selectNumDefenseDice(Country* defendingCountry); // randomly select number of defense dice
	int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled); // randomly select number of armies to move

	// Fortify methods
	virtual bool decideToFortify(); // randomly decide to fortify (if possible)
	Country* selectFortifyDestination(); // randomly select a country to fortify
	Country* selectFortifySource(Country* destination); // randomly select a neighboring country to fortify from
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination); // randomly select number of armies to move from source to destination
	
protected:
	// REINFORCE
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with least armies
	virtual int promptCountryToReinforce(); // prompt which country to reinforce
	virtual int promptNumOfArmiesToPlace(int totalArmies); // prompt how many armies to place
};

class CheaterPlayerStrategy : public PlayerStrategy
{
public:
	// Constructors, destructor
	CheaterPlayerStrategy();
	CheaterPlayerStrategy(PhaseLog* phaseLogPtr);
	CheaterPlayerStrategy(const CheaterPlayerStrategy& strategy);
	virtual ~CheaterPlayerStrategy();
	// Assignment operator
	CheaterPlayerStrategy& operator =(const CheaterPlayerStrategy& rightSide);

	bool isCheater() { return true; } // indicates whether a strategy is a cheater strategy

	// REINFORCE
	bool ifPlayerWantsToExchange(); // If have >= 3 cards, make a 50/50 decision to exchange or not.
	int promptExchangeForArmies(bool isMandatory); // Prompt user to choose which cards to exchange and get armies received
	void distributeArmies(int totalArmies); // Reinforce by doubling every owned country's armies

	// ATTACK
	void cheaterAttack(); // Attack method specifically for the cheater strategy

	// FORTIFY
	void cheaterFortify(); // Fortify method specifically for the fortify strategy

protected:
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with least armies
};