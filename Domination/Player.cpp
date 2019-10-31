#include "Player.h"

#include <iostream>
using namespace std;

//constructor, destructor
Player::Player() : id(new int(*currentGenId)), name(new string("Player " + to_string(*id))), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int, int>), hand(NULL), dice(new Dice()) {	genNextID(); }
Player::Player(string name, Deck *deck, Map *mapPtr) : id(new int(*currentGenId)), name(new string(name)), ownedCountries(new map<int, Country*>), numOfOwnedCountriesPerContinent(new map<int,int>), mapPtr(mapPtr), hand(new Hand(deck, ownedCountries)), dice(new Dice()){ genNextID(); }
Player::Player(const Player &p) : id(p.id), name(p.name), ownedCountries(p.ownedCountries), numCountriesInContinent(p.numCountriesInContinent), hand(p.hand), dice(p.dice) {}
Player::~Player() { delete id, name, ownedCountries, numOfOwnedCountriesPerContinent, hand, dice; }

//used in the startup phase of the game. Stores all owned countries and places 1 army on each
// And sets how many owned countries are in each continent for reinforcing armies.
void Player::setOwnedCountries(list<Country*> countriesList) 
{ 
	for (Country* country : countriesList) {
		country->playerId = *id;
		country->armies = 1;
		(*ownedCountries)[country->id] = country;
		(*numOfOwnedCountriesPerContinent)[country->continentId] += 1;
	}
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

/*	Get armies from owning any continent.
	@return army reinforcement from owned continent
*/
int Player::getContinentReinforcement() {
	
	int reinforcements = 0;
	list<Continent> allContinents = mapPtr->getContinents();
	
	for(Continent c : allContinents) {
		if (c.getSize() == (*numOfOwnedCountriesPerContinent)[c.getId])
			reinforcements += c.getWorth();
	}
	return reinforcements;
}

//used during attack(). Adds this country to the ones owned by the player, places on it this many armies.
void Player::claimCountry(Country* country, int armies)
{
	country->playerId = *id;
	country->armies = armies;
	(*ownedCountries)[country->id] = country;
	(*numOfOwnedCountriesPerContinent)[country->id] += 1;
}

//used during attack(). Returns a pointer to the lost country so that another player can add it to their collection. Returns nullptr if the country with this id is not owned.
Country* Player::loseCountry(int id)
{
	if (ownedCountries->count(id) == 0)
		return nullptr;
	else {
		(*numOfOwnedCountriesPerContinent)[id] -= 1;

		Country* country = (*ownedCountries)[id];
		ownedCountries->erase(id);
		country->playerId = -1;
		country->armies = -1;
		return country;
	}
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

/*	Get reinforcement armies and let player distribute given armies.
*/
void Player::reinforce() {

	int reinforcements = 0, r = 0;
	reinforcements += getCountryReinforcement();
	reinforcements += getContinentReinforcement(); 

	while(hand->getHandCount() >= 3) {
		r += hand->exchange();
		if(r == 0)	// note 0 means user cancelled exchange action
			break;
		else if(hand->getHandCount() >= 3)
			cout << "\nYou still have " << hand->getHandCount() << " left.\n"; 
	}
	reinforcements += r;
	// prompt to distribute reinforcements
}

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

/*	Return number of owned countries.
*/
int Player::getNumOfOwnedCountries() {
	return ownedCountries->size();
}