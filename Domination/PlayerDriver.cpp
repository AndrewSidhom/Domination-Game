/*#include "Player.h"

#include <iostream>
using namespace std;

void display(map<int, int> ownedArmies);

int main() {
	int id;
	cout << "Enter player id:" << endl;
	cin >> id;

	string name;
	cout << "Enter player name:" << endl;
	cin >> name;

	Player p(id, name);

	cout << endl;
	cout << "Creating 3 countries..." << endl;
	Country* c1 = new Country(1,1,"Canada");
	Country* c2 = new Country(2, 1, "United States");
	Country* c3 = new Country(3, 1, "Mexico");
	cout << endl;

	cout << "After each of the following operations, the player's owned countries and armies will be displayed." << endl;
	cout << "Setting the player's owned countries to be country 1 and 2..." << endl;
	map<int, Country*> owned;
	owned[c1->id] = c1;
	owned[c2->id] = c2;
	p.setOwnedCountries(owned);
	display(p.getOwnedArmies());

	cout << "The player obtains country 3 and places on it 1 army..." << endl;
	p.claimCountry(c3, 1);
	display(p.getOwnedArmies());

	cout << "The player loses country 1..." << endl;
	p.loseCountry(1);
	display(p.getOwnedArmies());

	cout << "The player tries to remove 1 army from country 3..." << endl;
	try {
		p.addOrRemoveArmies(3, -1);
	}
	catch (invalid_argument e) {
		cout << e.what() <<endl;
	}
	display(p.getOwnedArmies());
	
	cout << "The player adds 2 armies to country 3..." << endl;
	try {
		p.addOrRemoveArmies(3, 2);
	}
	catch (invalid_argument e) {
		cout << e.what() << endl;
	}
	display(p.getOwnedArmies());


	cout << endl; 
	cout << "********************************************************************************************" << endl;
	int armies2, armies3;
	cout << "Reinforce country 2 with how many armies?" << endl;
	cin >> armies2;
	cout << "Reinforce country 3 with how many armies?" << endl;
	cin >> armies3;
	cout << endl;
	map<int, int> reinforcements;
	reinforcements[2] = armies2;
	reinforcements[3] = armies3;
	p.reinforce(reinforcements);
	cout << endl;

	int from, to;
	cout << "Choose country id from which to attack" << endl;
	cin >> from;
	cout << "Choose country id to attack" << endl;
	cin >> to;
	p.attack(from, to);
	cout << endl;

	int armies;
	cout << "Choose country id from which to fortify" << endl;
	cin >> from;
	cout << "Choose country id to fortify" << endl;
	cin >> to;
	cout << "Choose number of armies to move" << endl;
	cin >> armies;
	p.fortify(from, to, armies);
	cout << endl;

	system("pause");
}

void display(map<int, int> ownedArmies) {
	for (const auto& kv : ownedArmies) {
		cout << "Country " << kv.first << " : " << kv.second << " armies." << endl;
	}
	cout << endl;
}
*/