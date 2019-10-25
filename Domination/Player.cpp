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
void Player::claimCountry(Country* country, int armies)
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

// Base method. Get reinforcement armies and let player distribute given armies.
void Player::reinforce(Deck* deck) {}

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
	
	if(handPtr->getHandCount() >= 5) {
		//reinforcements += handPtr->exchange(getOwnedCountries(), deck, true);
		if(handPtr->getHandCount() >= 3) 
			cout << "\nYou still have " << handPtr->getHandCount() << " left.\n";
	}

	if(handPtr->getHandCount() >= 3) {
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