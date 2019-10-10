#include "Player.h"

#include <iostream>
using namespace std;

//constructor, destructor
Player::Player(int id, string name): id(new int(id)), name(new string(name)), ownedCountries(nullptr), ownedArmies(new map<int,int>()), hand(new Hand()), dice(new Dice()){}
Player::~Player() { delete id; delete name; delete ownedCountries; delete ownedArmies; delete hand; delete dice; }

//used in the set-up phase of the game. Also sets ownedArmies with 1 army on each owned country.
void Player::setOwnedCountries(map<int, Country*> ownedCountries_) 
{ 
	this->ownedCountries = new map<int, Country*>(ownedCountries_); 
	for (const auto& kv : ownedCountries_) {
		(*ownedArmies)[kv.first] = 1;
	}
}

//adds this country to the ones owned by the player, places on it this many armies 
void Player::obtainCountry(Country* country, int armies)
{
	int id = country->id;
	(*ownedCountries)[id] = country;
	(*ownedArmies)[id] = armies;
}

//returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned.
Country* Player::loseCountry(int id)
{
	if (ownedCountries->count(id) == 0)
		return nullptr;
	else {
		Country* c = (*ownedCountries)[id];
		ownedCountries->erase(id);
		ownedArmies->erase(id);
		return c;
	}
}

//armies can be a +ve or -ve integer, meaning add or remove this many armies. THROWS EXCEPTION if country is not owned or if the number of armies to remove >= current number of armies.
void Player::addOrRemoveArmies(int countryId, int armies)
{
	if (ownedArmies->count(countryId) == 0)
		throw invalid_argument("Could not add/remove armies because the country with id " + to_string(countryId) + " is not owned by Player " + to_string(*id));
	else {
		int currentArmies = (*ownedArmies)[countryId];
		if (currentArmies + armies < 1)
			throw invalid_argument("Could not remove " + to_string(abs(armies)) + " armies from country with id " + to_string(countryId) + " because it currently has " + to_string(currentArmies) + " armies.");
		else
			(*ownedArmies)[countryId] = currentArmies + armies;
	}
}

//rolls this number of dice, returns dice results
vector<int> Player::rollDice(int howMany)
{
	//needs the Dice class to provide a roll() function that takes the number of dice as parameter. 
	return vector<int>();
}

//parameter: a mapping from country ids to the desired number of armies to add on each
void Player::reinforce(map<int, int> reinforcements)
{
	cout << "Reinforcing countries as follows:" << endl;
	for (const auto& kv : reinforcements) 
		cout << "Country " << kv.first << " with " << kv.second << " armies." << endl;
}

//an attack "from" an owned country "to" a country of another player
void Player::attack(int from, int to)
{
	cout << "Attacking from country " << from << " to country " << to << endl;
}

//move this many armies "from" an owned country "to" another owned country
void Player::fortify(int from, int to, int armies)
{
	cout << "Fortifying from country " << from << " to country " << to << ". Moving " << armies << " armies." << endl;
}





