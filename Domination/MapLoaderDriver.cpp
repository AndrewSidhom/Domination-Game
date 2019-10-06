#include <iostream>
#include "MapLoader.h"
using namespace std;

int main() {
	/*
		******************* A simple valid map ****************************
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


	/*
		******************* A valid map taken from the Domination game source files ****************************
	*/

	Map* amerokiMap = MapLoader::loadMapFile("ameroki");

	cout << endl << "**********************************************************" << endl;
	cout << "Loading ameroki.map" << endl;

	// Print all the continents
	cout << endl << "------- List of continents of ameroki.map --------";
	list<Continent> amContinents = amerokiMap->getContinents();
	for (auto& i : amContinents) {
		cout << endl << "Continent name: " << i.getName() << endl;
		cout << "Id: " << i.getId() << endl;
		cout << "Worth: " << i.getWorth() << endl;
	}
	cout << "-------- End of list ---------" << endl;

	// Print all the countries
	cout << endl << "------- List of countries of ameroki.map --------";
	list<Country> amCountries = amerokiMap->getCountries();
	for (auto& i : amCountries) {
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


	/*
		**************************** Invalid map files (files with wrong syntax/format) *******************************
	*/

	// File does not exist 
	cout << endl << "**********************************************************" << endl;
	cout << "Loading doesNotExist.map" << endl << endl;
	Map* invalidMap1 = MapLoader::loadMapFile("doesNotExist");
	cout << "**********************************************************" << endl;

	// Continents are defined before Map name
	cout << endl << "**********************************************************" << endl;
	cout << "Loading continentsBeforeName.map" << endl << endl;
	Map* invalidMap2 = MapLoader::loadMapFile("continentsBeforeName");
	cout << "**********************************************************" << endl;

	// Countries are defined before Continents
	cout << endl << "**********************************************************" << endl;
	cout << "Loading countriesBeforeContinents.map" << endl << endl;
	Map* invalidMap3 = MapLoader::loadMapFile("countriesBeforeContinents");
	cout << "**********************************************************" << endl;

	// Borders are defined before Countries
	cout << endl << "**********************************************************" << endl;
	cout << "Loading bordersBeforeCountries.map" << endl << endl;
	Map* invalidMap4 = MapLoader::loadMapFile("bordersBeforeCountries");
	cout << "**********************************************************" << endl;

	// File containing an invalid Continent (there is a string instead of an int on line 6)
	cout << endl << "**********************************************************" << endl;
	cout << "Loading invalidContinent.map" << endl << endl;
	Map* invalidMap5 = MapLoader::loadMapFile("invalidContinent");
	cout << "**********************************************************" << endl;

	// File containing an invalid Country (a country has a continent id that corresponds to no existing continent)
	cout << endl << "**********************************************************" << endl;
	cout << "Loading invalidCountry.map" << endl << endl;
	Map* invalidMap6 = MapLoader::loadMapFile("invalidCountry");
	cout << "**********************************************************" << endl;

	// File containing an invalid Border (a border definition contains a string)
	cout << endl << "**********************************************************" << endl;
	cout << "Loading invalidBorder.map" << endl << endl;
	Map* invalidMap7 = MapLoader::loadMapFile("invalidBorder");
	cout << "**********************************************************" << endl;

	cout << endl;
	system("pause");
}