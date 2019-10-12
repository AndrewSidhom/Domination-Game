#include "MapLoader.h"
#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Initialize static data members
int* MapLoader::continentId = new int(1);
string* MapLoader::processedSection = new string("");
string* MapLoader::mapName = new string("");

// If any issue was encountered while loading the map file, a nullptr is returned
Map* MapLoader::loadMapFile(std::string fileName) {
	// Create the file reading stream
	ifstream inputFile("maps/" + fileName + ".map");

	// Check if the file could be opened
	if (inputFile.fail()) {
		cout << "Opening the file " << fileName << ".map in the /maps directory failed." << endl;
		return nullptr;
	}

	Map* aMap = nullptr;
	// Reset static data members
	*continentId = 1;
	*processedSection = "";
	*mapName = "";

	string line;
	stringstream lineStream;
	string lineItem;

	// Read the file line by line
	while (getline(inputFile, line)) {
		// Check which [section] is being processed right now
		try {
			if (processSection(line)) {
				continue;
			}
		} catch (const exception e) {
			cout << e.what() << endl;
			return nullptr;
		}
		
		// If line is empty, skip it
		if (line == "") {
			continue;
		}

		// If no specific [section] is being processed right now,
		// check if line contains the map name which should be defined before any [section]
		if (processedSection->length() == 0) {
			stringstream lineStream(line);
			string lineItem;
			getline(lineStream, lineItem, ' ');

			if (lineItem == "name") {
				getline(lineStream, lineItem, ' ');
				*mapName = lineItem;
				aMap = new Map(*mapName);
			}
		} else if (*processedSection == "continents") {
			try {
				aMap->addContinent(processContinent(line));
			} catch (const invalid_argument e) {
				cout << "An invalid continent definition was found." << endl;
				delete(aMap);
				return nullptr;
			}
		} else if (*processedSection == "countries") {
			try {
				aMap->addCountry(processCountry(line, aMap));
			} catch (...) {
				cout << "An invalid country definition was found." << endl;
				delete(aMap);
				return nullptr;
			}
		} else if (*processedSection == "borders") {
			int countryId;

			stringstream lineStream(line);
			string lineItem;

			getline(lineStream, lineItem, ' ');

			// Get the Country for which the borders are being processed
			try {
				countryId = stoi(lineItem);
			} catch (const invalid_argument e) {
				cout << "An invalid border definition was found." << endl;
				delete(aMap);
				return nullptr;
			}

			Country* aCountry = nullptr;
			try {
				aCountry = aMap->getCountryById(countryId);
			} catch (const invalid_argument e) {
				cout << "An invalid country ID was found in a border definition." << endl;
				delete(aMap);
				return nullptr;
			}
			
			try {
				processBorder(line, aCountry);
			} catch (...) {
				cout << "An invalid border definition was found." << endl;
				delete(aMap);
				return nullptr;
			}
		}
	}

	inputFile.close();

	if (!aMap->validate()) {
		cout << "The map information read from " << fileName << ".map is invalid." << endl;
		delete(aMap);
		return nullptr;
	} else {
		cout << "The " << *mapName << " map is valid and has been properly loaded." << endl;
	}

	return aMap;
}

// If a recognized [section] is found, the processedSection is updated accordingly
// Only the following processing order is accepted: map name > [continents] > [countries] > [borders]
// Any other order is considered invalid and will return false
bool MapLoader::processSection(string line) {
	if (line == "[continents]") {
		if (*mapName == "") {
			throw exception("Invalid file syntax; missing map name.");
		}

		*processedSection = "continents";
		return true;
	}
	
	if (line == "[countries]") {
		if (*processedSection != "continents") {
			throw exception("Invalid file syntax; no continents were defined before defining countries.");
		}

		*processedSection = "countries";
		return true;
	}
	
	if (line == "[borders]") {
		if (*processedSection != "countries") {
			throw exception("Invalid file syntax; no countries were defined before defining borders.");
		}

		*processedSection = "borders";
		return true;
	}

	return false;
}

// Returns a Continent with a generated id, and a name and worth read from line
Continent MapLoader::processContinent(string line) {
	string continentName;
	int continentWorth;

	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	continentName = lineItem;

	getline(lineStream, lineItem, ' ');
	continentWorth = stoi(lineItem);

	Continent aContinent = Continent(*continentId, continentName, continentWorth);

	*continentId = *continentId + 1;

	return aContinent;
}

// Returns a Country with the id, continent id and name read from line
Country MapLoader::processCountry(string line, Map* aMap) {
	int countryId;
	string countryName;
	int countryContinentId;

	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	countryId = stoi(lineItem);

	try {
		aMap->getCountryById(countryId);
		throw exception("There are duplicate countries.");
	} catch (const invalid_argument e) {
		// do nothing, this is actually the good case; the country does not already exist
	}

	getline(lineStream, lineItem, ' ');
	countryName = lineItem;

	getline(lineStream, lineItem, ' ');
	countryContinentId = stoi(lineItem);

	return Country(countryId, countryContinentId, countryName, list<int>());
}

// Updates a Country's neighbour list according to the borders specified in line
void MapLoader::processBorder(string line, Country* aCountry) {
	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	int countryId = stoi(lineItem);

	while (getline(lineStream, lineItem, ' ')) {
		if (countryId == stoi(lineItem)) {
			throw exception("A country cannot be its own neighbour.");
		}
		aCountry->neighbors.push_back(stoi(lineItem));
	}
}
