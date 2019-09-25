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
	int* id;
	string* name;
	list<int>* neighbors;  //ids of countries adjacent to this one

public:
	Country(int id, string name, list<int>* neighbors)
};



//Representation of a continent
class Continent {
	int* id;
	string* name;
	list<Country*>* countries;
	Graph* innerGraph;  //a graph for only this continent's countries. This graph is constructed during execution of the continent's validate() method.
	list<int>* neighbors;  //ids of continents adjacent to this one. This list is constructed during execution of the continent's validate() method.
	bool* validated;  //true if the continent's validate() method has been called.
	bool* isValid;  //true if the continent's validate() method has been called and found that the continent is valid for the game (its innerGraph is connected)

public:
	//accessors...
	int getId();
	string getName();
	list<Country*> getCountries();
	Graph getInnerGraph();
	list<int> getNeighbors();
	bool getValidated();
	bool getIsValid();

	//constructor
	Continent(int id, string name);

	//methods
	void addCountry(Country*); //Note: the method will not add the country if continent has already been validated and found valid, at which point its list of countries is final.
	bool validate(); //Constructs the continent's "innerGraph" and "neighbors", checks if innerGraph is connected, sets the isValid variable accordingly.
};



//Representation of a map
class Map {
	int* id;
	string* name;
	list<Continent*>* continents;
	Graph* continentsGraph;  //a graph where each node is a continent. This graph is constructed during execution of the map's validate() method.
	list<Countries*>* countries;
	bool* validated;  //true if the map's validate() method has been called.
	bool* isValid;  //true if the map's validate() method has been called and found that the map is valid for the game.

public:
	//accessors...
	int getId();
	string getName();
	list<Continent*> getContinents();
	Graph getContinentsGraph();
	list<Country*> getCountries;
	bool getValidated();
	bool getIsValid();

	//constructor
	Map(int id, string name, list<Continent*>* continents);

	//methods
	bool validate(); //calls each continent's validate() method, proceeds if valid, constructs the map's continentsGraph, checks if it is connected which means the whole map is valid. Sets the isValid variable accordingly.
	Country* getCountryById(int id);
};