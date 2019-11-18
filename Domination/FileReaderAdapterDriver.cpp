//#include "MapLoader.h"
//#include <iostream>
//using namespace std;
//
//void printMap(Map* gameMap);
//
//int main() {
//	ConquestMapLoader cml;
//	Map* gameMap;
//	char repeat = 'n';
//	do {
//		int type;
//		cout << "Enter 1 or 2 for the type of map you would like to load" << endl;
//		cout << "1. Domination map" << endl;
//		cout << "2. Conquest map" << endl;
//		cin >> type;
//		while (type != 1 && type != 2) {
//			cout << "Invalid number entered. Try again." << endl;
//			cin >> type;
//		}
//		string mapName;
//		cout << "Enter the name of the map you would like to load (without the .map extension): " << endl;
//		cin >> mapName;
//		if (type == 1)
//			gameMap = MapLoader::loadMapFile(mapName);
//		else
//			gameMap = ConquestMapAdapter::loadMapFile(mapName);
//		while (gameMap == nullptr) {
//			cout << "Invalid map name, try again: " << endl;
//			cin >> mapName;
//			if (type == 1)
//				gameMap = MapLoader::loadMapFile(mapName);
//			else
//				gameMap = ConquestMapAdapter::loadMapFile(mapName);
//		}
//		printMap(gameMap);
//		cout << endl;
//		cout << "Would you like to load another map? (y/n)" << endl;
//		cin >> repeat;
//	} while (repeat=='y');
//	
//	return 0;
//}
//
//void printMap(Map * gameMap) {
//	// Print all the continents
//	cout << endl << "------- List of continents of valid.map --------";
//	list<Continent> vmContinents = gameMap->getContinents();
//	for (auto& i : vmContinents) {
//		cout << endl << "Continent name: " << i.getName() << endl;
//		cout << "Id: " << i.getId() << endl;
//		cout << "Worth: " << i.getWorth() << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//
//	// Print all the countries
//	cout << endl << "------- List of countries of valid.map --------";
//	list<Country*> vmCountries = *(gameMap->getCountries());
//	for (auto& i : vmCountries) {
//		cout << endl << "Country name: " << i->name << endl;
//		cout << "Id: " << i->id << endl;
//		cout << "Continent id: " << i->continentId << endl;
//
//		cout << "Neighbors: ";
//		list<int> neighbors = i->neighbors;
//		for (auto& j : neighbors) {
//			cout << j << " ";
//		}
//		cout << endl;
//	}
//	cout << "-------- End of list ---------" << endl;
//	cout << "**********************************************************" << endl;
//}