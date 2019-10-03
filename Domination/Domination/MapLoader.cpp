#include "MapLoader.h"
#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

//TODO: throw error if map file order not respected (such as countries defined before any continent is defined)
//TODO: throw error at any point that the syntax of the map file does not correspond to expected syntax

void MapLoader::loadMapFile(std::string fileName) {
	ifstream inputFile("test.map.txt");

	continentId = new int(1);
	processedSection = new string("");
	mapName = new string("");

	string line;
	stringstream lineStream;
	string lineItem;
	while (getline(inputFile, line)) {
		if (line.compare("[continents]") == 0) {
			*processedSection = "continents";
			continue;
		} else if (line.compare("[countries]") == 0) {
			*processedSection = "countries";
			continue;
		}

		if (line == "") {
			continue;
		}

		if ((*processedSection).compare("") == 0) {
			stringstream lineStream(line);
			string lineItem;
			getline(lineStream, lineItem, ' ');

			if (lineItem.compare("name") == 0) {
				getline(lineStream, lineItem, ' ');
				*mapName = lineItem;
			}
		}

		if ((*processedSection).compare("continents") == 0) {
			processContinent(line);
		} else if ((*processedSection).compare("countries") == 0) {
			processCountry(line);
		}
	}

	inputFile.close();

	return;
}

void MapLoader::processContinent(string line) {
	string continentName;
	int continentWorth;

	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	continentName = lineItem;

	getline(lineStream, lineItem, ' ');
	continentWorth = stoi(lineItem);

	//TODO: create continent and add it to list of continents

	//Continent aContinent = Continent(*continentId, continentName, continentWorth);
	
	*continentId++;

	//return aContinent;
}

void MapLoader::processCountry(string line) {
	int countryId;
	string countryName;
	int countryContinentId;

	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	countryId = stoi(lineItem);

	getline(lineStream, lineItem, ' ');
	countryName = lineItem;

	getline(lineStream, lineItem, ' ');
	countryContinentId = stoi(lineItem);

	//TODO: create country struct with name and id, but empty list of neighbours
	//TODO: add country to list of countries of a continent and to overall list of countries
}

void MapLoader::processBorder(string line) {
	int countryId;

	stringstream lineStream(line);
	string lineItem;

	getline(lineStream, lineItem, ' ');
	countryId = stoi(lineItem);

	while (getline(lineStream, lineItem, ' ')) {
		//TODO: Find country with current id and add it to current country's neighbours
	}
}
