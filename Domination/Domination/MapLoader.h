#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class MapLoader {

public:
	void loadMapFile(string fileName);

private:
	void processMapName(string name);
	void processContinent(string data);
	void processCountry(string data);
	void processBorder(string data);

};
