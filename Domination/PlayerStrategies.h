#pragma once
#include "Map.h"
#include <vector>
using std::vector;

// forward-declaration to resolve circular dependency
class Player;
class Hand;

// The Player Strategy to be inherited by Concrete Stragies. Its default behaviour is that of a HUMAN Player 
// (requests for input for any kind of decision)
class PlayerStrategy {
public:
	//don't forget constructors, etc.!!
	const int MIN_NUM_ATTACK_DICE = 1;
	const int MIN_NUM_DEFENSE_DICE = 1;
	const int MAX_NUM_DEFENSE_DICE = 2;
	const int MIN_NUM_ARMIES_ON_COUNTRY = 1;

	PlayerStrategy();
	PlayerStrategy(Player* aPlayer, Hand* aHand);
	PlayerStrategy(const PlayerStrategy& strategy);
	virtual ~PlayerStrategy();

	const PlayerStrategy& operator =(const PlayerStrategy& rightSide);

	void setPlayer(Player* aPlayer) { player = aPlayer; };
	void setHand(Hand* handPtr) { hand = handPtr; };

	// REINFORCE
	virtual bool ifPlayerWantsToExchange(); // Prompt if user wants to exchange
	virtual int promptExchangeForArmies(bool isMandatory); // Prompt user to choose which cards to exchange and get armies received

	// ATTACK
	virtual void attackInit(); // Do whatever initialization is required before deciding to attack or not
	virtual bool decideToAttack(); // Choose whether to attack an enemy country or not
	virtual Country* selectAttackingCountry(); // Choose which Country to attack from. This Country should have at least 2 armies within it and should have neighbouring enemy Countries
	virtual Country* selectDefendingCountry(Country* attackingCountry); // Choose a Country to attack from the attacking Country
	virtual int selectNumAttackDice(Country* attackingCountry); // Choose the number of dice to roll for attack
	virtual int selectNumDefenseDice(Country* defendingCountry); // Choose the number of dice to roll for defending against an attack
	virtual int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled); // Choose the number of armies to move from the attackingCountry to the defendingCountry after the Player won an attack

	// FORTIFY
	virtual bool canFortify(); // Whether a Player has a Country that can be fortified
	bool isValidFortifyDestination(Country* destination); // Returns true if a Country is a valid fortification destination
	bool isValidFortifiySource(Country* destination, Country* source); // Returns true if a Country is a valid fortification source for a given destination
	virtual bool decideToFortify(); // Choose whether to fortify a Country or not
	virtual Country* selectFortifyDestination(); // Choose which Country should be fortified
	virtual Country* selectFortifySource(Country* destination); // Choose a Country from which to move armies into the destination Country
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination); // Choose how many armies to move from the source Country into the destination Country

protected:
	Player* player;
	Hand* hand;

	int exchangeAnyCardsForArmies(); // if have cards to exchange, return exchange armies; else return 0 (has no user input - mostly for AI)
	void tradeInCards(int* cardsToExchange); // After choosing exchanged cards, trade in the cards
	virtual void distributeExchangeBonus(vector<int>* matchingCountries); //	Prompt user to choose which country that matches the exchanged cards to receive +2 bonus armies.

	// RELATED SERVICE FUNCS
	int genRandomNum(int low, int high); // generates a random number in between range

private:
	int getPlayersCardOfChoice(bool isMandatory, int numOfCardsChosen, int cardsToExchangeIndex[]); //	Prompt user to choose which card from their hand to exchange.
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:

	AgressivePlayerStrategy();
	AgressivePlayerStrategy(Player* aPlayer);
	AgressivePlayerStrategy(const AgressivePlayerStrategy& strategy);
	virtual ~AgressivePlayerStrategy();

	const AgressivePlayerStrategy& operator =(const AgressivePlayerStrategy& rightSide);

	// Strategy: reinforce strongest country
	bool ifPlayerWantsToExchange(); // Returns true when have cards >= 3
	int promptExchangeForArmies(bool isMandatory); // Automatically choose any cards in hands to exchange (has no user input)

	// Goal: Continue attacking until the strongest Country has no more Countries it can attack
	void attackInit(); // Find the strongest Country owned by the Player (Country with most armies) that can attack other Countries
	bool decideToAttack();
	Country* selectAttackingCountry(); // Returns the strongest Country
	Country* selectDefendingCountry(Country* attackingCountry); // Returns a neighbour of the strongest Country that can be attacked
	int selectNumAttackDice(Country* attackingCountry);
	int selectNumDefenseDice(Country* defendingCountry);
	int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled);

	virtual bool decideToFortify();
	Country* selectFortifyDestination();
	Country* selectFortifySource(Country* destination);
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination);

protected:
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with most armies

private:
	Country* strongestAttackingCountry;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(Player* aPlayer, Hand* aHand);
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& strategy);
	virtual ~BenevolentPlayerStrategy();

	const BenevolentPlayerStrategy& operator =(const BenevolentPlayerStrategy& rightSide);
	
	// Strategy: reinforce weakest countries
	bool ifPlayerWantsToExchange(); // Returns true when have cards >= 3
	int promptExchangeForArmies(bool isMandatory); // Automatically choose any cards in hands to exchange (has no user input)

	bool decideToAttack();

	virtual bool decideToFortify();
	Country* selectFortifyDestination();
	Country* selectFortifySource(Country* destination);
	virtual int selectArmiesToMoveForFortification(Country* source, Country* destination);

protected:
	void distributeExchangeBonus(vector<int>* matchingCountries); // Choose country with least armies
};
