#pragma once
#include "Map.h"
#include "Player.h"

// The Player Strategy to be inherited by Concrete Stragies. Its default behaviour is that of a HUMAN Player 
// (requests for input for any kind of decision)
class PlayerStrategy {
public:
	//don't forget constructors, etc.!!
	const int MIN_NUM_ATTACK_DICE = 1;
	const int MIN_NUM_DEFENSE_DICE = 1;
	const int MAX_NUM_DEFENSE_DICE = 2;

	void setPlayer(Player* aPlayer) { player = aPlayer; };

	virtual void attackInit(); // Do whatever initialization is required before deciding to attack or not
	virtual bool decideToAttack(); // Choose whether to attack an enemy country or not
	virtual Country* selectAttackingCountry(); // Choose which Country to attack from. This Country should have at least 2 armies within it and should have neighbouring enemy Countries
	virtual Country* selectDefendingCountry(Country* attackingCountry); // Choose a Country to attack from the attacking Country
	virtual int selectNumAttackDice(Country* attackingCountry) ; // Choose the number of dice to roll for attack
	virtual int selectNumDefenseDice(Country* defendingCountry); // Choose the number of dice to roll for defending against an attack
	virtual int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled); // Choose the number of armies to move from the attackingCountry to the defendingCountry after the Player won an attack

protected:
	Player* player;
};

class AgressivePlayerStrategy : public PlayerStrategy {
public:
	AgressivePlayerStrategy(); // don't forget constructors, etc.!!

	// Goal: Continue attacking until the strongest Country has no more Countries it can attack
	void attackInit(); // Find the strongest Country owned by the Player (Country with most armies) that can attack other Countries
	bool decideToAttack();
	Country* selectAttackingCountry(); // Returns the strongest Country
	Country* selectDefendingCountry(Country* attackingCountry); // Returns a neighbour of the strongest Country that can be attacked
	int selectNumAttackDice(Country* attackingCountry);
	int selectNumDefenseDice(Country* defendingCountry);
	int selectNumArmiesToMoveAfterAttackSuccess(Country* attackingCountry, Country* defendingCountry, int diceRolled);

private:
	Country* strongestCountry;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	bool decideToAttack();
};
