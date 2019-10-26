#include "Player.h"

#include <iostream>
using namespace std;

//constructor, destructor
Player::Player(int id, string name): id(new int(id)), name(new string(name)), ownedCountries(new map<int, Country*>), hand(new Hand()), dice(new Dice()){}
Player::~Player() { delete id; delete name; delete ownedCountries; delete hand; delete dice; }

//used in the startup phase of the game. Stores all owned countries and places 1 army on each.
void Player::setOwnedCountries(list<Country*> countriesList) 
{ 
	for (Country* country : countriesList) {
		(*ownedCountries)[country->id] = country;
		country->playerId = *id;
		country->armies = 1;
	}
}

//used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned.
Country* Player::loseCountry(int id)
{
	if (ownedCountries->count(id) == 0)
		return nullptr;
	else {
		Country* country = (*ownedCountries)[id];
		ownedCountries->erase(id);
		country->playerId = -1;
		country->armies = -1;
		return country;
	}
}

//used during attack(). Adds this country to the ones owned by the player, places on it this many armies .
void Player::claimCountry(Country* country, int armies)
{
	(*ownedCountries)[country->id] = country;
	country->playerId = *id;
	country->armies = armies;
}

//armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.
//void Player::addOrRemoveArmies(int countryId, int armies)
//{
//	Country* country = map.getCountryById(countryId);
//	if (ownedCountries->count(countryId) == 0)
//		throw invalid_argument("Could not add/remove armies because the country with id " + to_string(countryId) + " is not owned by Player " + to_string(*id));
//	else {
//		int currentArmies = country->armies;
//		if (currentArmies + armies < 1)
//			throw invalid_argument("Could not remove " + to_string(abs(armies)) + " armies from country with id " + to_string(countryId) + " because it currently has " + to_string(currentArmies) + " armies.");
//		else
//			country->armies = currentArmies + armies;
//	}
//}

//rolls this number of dice, returns dice results
vector<int> Player::rollDice(int howMany)
{
	//needs the Dice class to provide a roll() function that takes the number of dice as parameter. 
	return vector<int>();
}

// Base method. Get reinforcement armies and let player distribute given armies.
void Player::reinforce(Deck* deck) {}

//the player carries out a number of attacks
void Player::attack()
{
	//TODO
}

//move a number of armies from an owned country to another owned neighboring country
void Player::fortify()
{
	//TODO
}

/*	Get armies from total countries divided by 3, removing fractions.
	@return army reinforcement from owned country count
*/
int Player::getCountryReinforcement() {

	int reinforcements = (ownedCountries->size() / 3);
	if(reinforcements < 3)
		reinforcements = 3;	// 3 being minimum

	return reinforcements;
}

// TODO imple
int Player::getContinentReinforcement() {
	
	int reinforcements = 0;
}

PlayerHuman::PlayerHuman(int id, string name) : Player(id, name) {}
PlayerHuman::~PlayerHuman() {}

/*	Overridden method. Get reinforcement armies and let player distribute given armies.
*/
void PlayerHuman::reinforce(Deck *deck) {

	int reinforcements = 0;
	reinforcements += getCountryReinforcement(); // inherited func
	reinforcements += getContinentReinforcement(); // inherited func

	Hand* handPtr = getHand();
	handPtr->showHand();
	
	if(handPtr->getHandCount() >= 5) {	// force exchange if have 5+ cards in hand
		//reinforcements += handPtr->exchange(getOwnedCountries(), deck, true);
		if(handPtr->getHandCount() >= 3) 
			cout << "\nYou still have " << handPtr->getHandCount() << " left.\n";
	}

	if(handPtr->getHandCount() >= 3) {	// ask for exchange if have 3+ cards in hand
		cout << "Would you like to exchange your cards? (y/n): ";
		string input = getValidExchangeInput();
		
		//if(input.compare("y") == 0) // 0 means equal
			//reinforcements += handPtr->exchange(getOwnedCountries(), deck, false);
	}
}

/*	Get valid input of string 'y' or 'n'
	@return string input of y or n
*/
string PlayerHuman::getValidExchangeInput() {

	string input;
	bool validInput = false;
	do {
		cout << "Would you like to exchange? (y/n)" << endl;
		cin >> input;
		
		if (!cin.good()) /// !good() when input isnt a string
		{
			cout << "\nInvalid input. Please try again.\n";
			cin.clear();		   /// clear error flag
			cin.ignore(100, '\n'); /// clear buffer
		}
		else if(input.compare("y") != 0 || input.compare("n") != 0)  // 0 means equal
			cout << "\nInput must be 'y' or 'n'\n";
		else
			validInput = true;
	} 
	while (!validInput);

	return input;
}