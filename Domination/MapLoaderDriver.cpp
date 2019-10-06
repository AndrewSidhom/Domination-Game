#include <iostream>
#include "MapLoader.h"
using namespace std;

int main() {
	/*
		Simple valid map
	*/
	Map* validMap = MapLoader::loadMapFile("valid");

	cout << "**********************************************************" << endl;
	cout << "Loading valid.map" << endl;

	// Print all the continents
	cout << endl << "------- List of continents of valid.map --------";
	list<Continent> vmContinents = validMap->getContinents();
	for (auto& i : vmContinents) {
		cout << endl << "Continent name: " << i.getName() << endl;
		cout << "Id: " << i.getId() << endl;
		cout << "Worth: " << i.getWorth() << endl;
	}
	cout << "-------- End of list ---------" << endl;

	// Print all the countries
	cout << endl << "------- List of countries of valid.map --------";
	list<Country> vmCountries = validMap->getCountries();
	for (auto& i : vmCountries) {
		cout << endl << "Country name: " << i.name << endl;
		cout << "Id: " << i.id << endl;
		cout << "Continent id: " << i.continentId << endl;

		cout << "Neighbors: ";
		list<int> neighbors = i.neighbors;
		for (auto& j : neighbors) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << "-------- End of list ---------" << endl;
	cout << "**********************************************************" << endl;

	cout << endl << "**********************************************************" << endl;
	

	cout << endl;
	system("pause");
}