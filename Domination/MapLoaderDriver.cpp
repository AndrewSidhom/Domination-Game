#include <iostream>
#include "MapLoader.h"
using namespace std;
//
///*
//	The MapLoaderDriver reads several .map files in the maps folder and asserts whether these maps are valid.
//	Valid maps are returned as Map objects and invalide maps are returned as nullptr.
//*/
//
//int main() {
//	/*
//		******************* A simple valid map ****************************
//	*/
//
//	cout << "**********************************************************" << endl;
//	cout << "Loading valid.map" << endl;
//
//	Map* validMap = MapLoader::loadMapFile("valid");
//
//	// Print all the continents
//	cout << endl << "------- List of continents of valid.map --------";
//	list<Continent> vmContinents = validMap->getContinents();
//	for (auto& i : vmContinents) {
//		cout << endl << "Continent name: " << i.getName() << endl;
//		cout << "Id: " << i.getId() << endl;
//		cout << "Worth: " << i.getWorth() << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//
//	// Print all the countries
//	cout << endl << "------- List of countries of valid.map --------";
//	list<Country> vmCountries = validMap->getCountries();
//	for (auto& i : vmCountries) {
//		cout << endl << "Country name: " << i.name << endl;
//		cout << "Id: " << i.id << endl;
//		cout << "Continent id: " << i.continentId << endl;
//
//		cout << "Neighbors: ";
//		list<int> neighbors = i.neighbors;
//		for (auto& j : neighbors) {
//			cout << j << " ";
//		}
//		cout << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//	cout << endl << "A nullptr has been returned: " << (validMap == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//
//	/*
//		******************* A valid map taken from the Domination game source files ****************************
//	*/
//
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading ameroki.map" << endl;
//
//	Map* amerokiMap = MapLoader::loadMapFile("ameroki");
//
//	// Print all the continents
//	cout << endl << "------- List of continents of ameroki.map --------";
//	list<Continent> amContinents = amerokiMap->getContinents();
//	for (auto& i : amContinents) {
//		cout << endl << "Continent name: " << i.getName() << endl;
//		cout << "Id: " << i.getId() << endl;
//		cout << "Worth: " << i.getWorth() << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//
//	// Print all the countries
//	cout << endl << "------- List of countries of ameroki.map --------";
//	list<Country> amCountries = amerokiMap->getCountries();
//	for (auto& i : amCountries) {
//		cout << endl << "Country name: " << i.name << endl;
//		cout << "Id: " << i.id << endl;
//		cout << "Continent id: " << i.continentId << endl;
//
//		cout << "Neighbors: ";
//		list<int> neighbors = i.neighbors;
//		for (auto& j : neighbors) {
//			cout << j << " ";
//		}
//		cout << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//	cout << endl << "A nullptr has been returned: " << (amerokiMap == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//
//	/*
//		**************************** Invalid map files (files with wrong syntax/format) *******************************
//	*/
//
//	// File does not exist 
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading doesNotExist.map" << endl << endl;
//	Map* invalidMap1 = MapLoader::loadMapFile("doesNotExist");
//	cout << "A nullptr has been returned: " << (invalidMap1 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Continents are defined before Map name
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading continentsBeforeName.map" << endl << endl;
//	Map* invalidMap2 = MapLoader::loadMapFile("continentsBeforeName");
//	cout << "A nullptr has been returned: " << (invalidMap2 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Countries are defined before Continents
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading countriesBeforeContinents.map" << endl << endl;
//	Map* invalidMap3 = MapLoader::loadMapFile("countriesBeforeContinents");
//	cout << "A nullptr has been returned: " << (invalidMap3 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Borders are defined before Countries
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading bordersBeforeCountries.map" << endl << endl;
//	Map* invalidMap4 = MapLoader::loadMapFile("bordersBeforeCountries");
//	cout << "A nullptr has been returned: " << (invalidMap4 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// File containing an invalid Continent (there is a string instead of an int on line 6)
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading invalidContinent.map" << endl << endl;
//	Map* invalidMap5 = MapLoader::loadMapFile("invalidContinent");
//	cout << "A nullptr has been returned: " << (invalidMap5 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// File containing an invalid Country (a country has a continent id that corresponds to no existing continent)
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading invalidCountry.map" << endl << endl;
//	Map* invalidMap6 = MapLoader::loadMapFile("invalidCountry");
//	cout << "A nullptr has been returned: " << (invalidMap6 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// File containing an invalid Border (a border definition contains a string)
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading invalidBorder.map" << endl << endl;
//	Map* invalidMap7 = MapLoader::loadMapFile("invalidBorder");
//	cout << "A nullptr has been returned: " << (invalidMap7 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	/*
//		**************************** Invalid map files (files with invalid map structures/design) *******************************
//	*/
//
//	// Map containing duplicate Countries (Countries with the same ID)
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading duplicateCountries.map" << endl << endl;
//	Map* invalidMap8 = MapLoader::loadMapFile("duplicateCountries");
//	cout << "A nullptr has been returned: " << (invalidMap8 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Map containing a Country which is defined as its own neighbour
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading countryIsOwnNeighbour.map" << endl << endl;
//	Map* invalidMap9 = MapLoader::loadMapFile("countryIsOwnNeighbour");
//	cout << "A nullptr has been returned: " << (invalidMap9 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Map containing a completely isolated Country
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading isolatedCountry.map" << endl << endl;
//	Map* invalidMap10 = MapLoader::loadMapFile("isolatedCountry");
//	cout << "A nullptr has been returned: " << (invalidMap10 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	// Map containing a Country that is isolated within its Continent (it is connected to a Country in
//	// another Continent but it is not connected to any Country within its own Continent)
//	cout << endl << "**********************************************************" << endl;
//	cout << "Loading isolatedCountryWithinContinent.map" << endl << endl;
//	Map* invalidMap11 = MapLoader::loadMapFile("isolatedCountryWithinContinent");
//	cout << "A nullptr has been returned: " << (invalidMap11 == nullptr) << endl;
//	cout << "**********************************************************" << endl;
//
//	cout << endl;
//	system("pause");
//}

//int main() {
//	//MapLoader::loadMapFile("valid");
//	ConquestMapLoader cml;
//	cml.loadConquestMapFile("Atlantis");
//	system("pause");
//	return 0;
//}