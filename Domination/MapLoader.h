#pragma once

#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class MapLoader {

public:
	void loadMapFile(string fileName);

private:
	int* continentId;
	string* processedSection;
	string* mapName;


	void processContinent(string line);
	void processCountry(string line);
	void processBorder(string line);

};
