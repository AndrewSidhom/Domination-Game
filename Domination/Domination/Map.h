#pragma once

#include <map>
#include <list>
#include <string>
using namespace std;



//Representation of a graph using adjacency lists. Nodes are referred to by ids. Each node maps to a list of its adjacent nodes.
class Graph {
	map<int, list<int> >* adjLists;

public:
	Graph(map<int, list<int> >* adjLists);
	bool isConnected();
};



//Representation of a country
struct Country {
	int id;
	string name;
	list<int> neighbors;  //ids of countries adjacent to this one

	Country(int id, string name, list<int> neighbors);
};



//Representation of a continent
class Continent {
	int* id;
	string* name;
	int* worth; //the number of armies a player gets if they acquire the whole continent 
	list<Country*>* countries;
	Graph* innerGraph;  //a graph for only this continent's countries. This graph is constructed during execution of the continent's validate() method.
	bool* validated;  //true if the continent's validate() method has been called.
	bool* isValid;  //true if the continent's validate() method has been called and found that the continent is valid for the game (its innerGraph is connected)

public:
	//accessors...
	int getId() { return *id; };
	string getName() { return *name; };
	int getWorth() { return *worth; };
	list<Country*> getCountries() { return *countries; };
	Graph getInnerGraph() { return *innerGraph; };
	bool getValidated() { return *validated; };
	bool getIsValid() { return *isValid; };

	//constructor and destructor
	Continent(int id, string name, int worth);
	~Continent();

	//methods
	void addCountry(Country* country); //Note: Country will NOT be added if continent has already been validated and found valid, at which point its list of countries is final.
	bool validate(); //Constructs the continent's "innerGraph" and "neighbors", checks if innerGraph is connected, sets the isValid variable accordingly.
};



//Representation of a map
class Map {
	int* id;
	string* name;
	list<Continent*>* continents;
	Graph* graph;  //a graph of the full map. Constructed during execution of the map's validate() method.
	list<Country*>* countries;
	bool* validated;  //true if the map's validate() method has been called.
	bool* isValid;  //true if the map's validate() method has been called and found that the map is valid for the game.

public:
	//accessors...
	int getId() { return *id; };
	string getName() { return *name; };
	list<Continent*> getContinents() { return *continents; };
	Graph getGraph() { return *graph; };
	list<Country*> getCountries() { return *countries; };
	bool getValidated() { return *validated; };
	bool getIsValid() { return *isValid; };

	//constructor and destructor
	Map(int id, string name, list<Continent*>* continents);
	~Map();

	//methods
	bool validate(); //calls each continent's validate() method, proceeds if valid, constructs the map's full graph, checks if it is connected. Sets the isValid variable accordingly.
	Country* getCountryById(int id);
};