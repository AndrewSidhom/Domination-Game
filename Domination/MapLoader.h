#pragma once

#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

/*
	The MapLoader is used for loading a map from a .map file and instantiate it into a Map object

	It is intended to be a TOOL and should NOT be instantiated
*/
class MapLoader {

public:
	// The method used for loading a .map file
	// The fileName should NOT include a file extension (ex.: "file" and not "file.map")
	// The fileName.map file is read from the maps folder. The maps folder should be at the same directory level as the MapLoader
	static Map* loadMapFile(string fileName);

private:
	static int* continentId; // Used for generating continent ids
	static string* processedSection; // The current [section] being processed in the .map file
	static string* mapName; // The name of the map

	static bool processSection(string line); // Checks which [section] is being processed. Returns true if [section] order is valid, false otherwise
	static Continent processContinent(string line); // Reads information on a Continent and instantiates one accordingly
	static Country processCountry(string line, Map* aMap); // Reads information on a Country and instantiates one accordingly
	static void processBorder(string line, Country* aCountry); // Reads information on a Country's borders and updates the Country accordingly

};

class ConquestMapLoader {
public:
	ConquestMapLoader(); // Constructor
	~ConquestMapLoader(); // Destructor
	Map *loadConquestMapFile(string fileName); // Reads Conquest file and instantiates a Map 

private:
	int* continentId; // Used for generating continent ids
	int* countryId; // To generate country ids
	string* processedSection; // The current [section] being processed in the .map file
	map<string, int> *continents; //in order to get continent ids by names and manage duplicates
	map<string, int> *countries; //in order to get country ids by names and manage duplicates

	bool processSection(string line); // Checks which [section] is being processed. Returns true if [section] order is valid, false otherwise
	bool processMapSection(string line); // Reads information on a Country and instantiates one accordingly
	Continent processContinent(string line); // Reads information on a Continent and instantiates one accordingly
	Country processTerritory(string line); // Reads information on a territoriy (country) and instantiates one accordingly

};

//Adapter class 
class ConquestMapAdapter : public MapLoader {
private: 
	static ConquestMapLoader* conquestMapLoader;
public:
	static Map* loadMapFile(string fileName);	
};