#pragma once

#include <map>
#include <list>
#include <string>
using namespace std;

class Player;


/* Intended usage of Map.h/Map.cpp ...
1. Create Map m.
2. Create Continent objects, calling m.addContinent(Continent c) each time.
3. Create Country objects, calling m.addCountry(Country c) each time.
4. Call m.validate()
5. If the result of validation is successful, use m for the game.
6. Use m.getCountryById(int id) and m.getContinentById(int id) as needed during the game.
*/




//Representation of a graph using adjacency lists. Nodes are referred to by ids. 
//Each node maps to a list of its adjacent nodes.
class Graph {
private:
	map<int, list<int> >* adjLists;

public:
	map<int, list<int> > getNodes();
	Graph();  //constructor
	~Graph();  //destructor
	Graph(const Graph& old); //copy constructor
	const Graph& operator=(const Graph& g); //assignment operator
	void addOrUpdateNode(int id, list<int> neighbors); //update if node with this id exists, add otherwise.
	bool isConnected();
};



//Representation of a country
struct Country {
	int id;
	int continentId;   //the continent this country belongs to
	string name;
	list<int> neighbors;  //ids of countries adjacent to this one
	Player* player; //player who owns the country during gameplay. Is a nullptr if undefined (for example before startup phase or while country is changing ownership)
	int armies; //number of armies present on the country during gameplay. Equals -1 if undefined (for example before startup phase or while country is changing ownership)

	Country();
	Country(int id, int continentId, string name);
	Country(int id, int continentId, string name, list<int> neighbors);
};



//Representation of a continent
class Continent {

private:
	int* id;
	string* name;
	int* worth; //the number of armies a player gets if they acquire the whole continent 
	int* size; //number of countries on the continent
	Graph* innerGraph;  //a graph for only this continent's countries.
	bool* validated;  //true if the continent's validate() method has been called.
	bool* isValid;  //true if the continent's validate() method has been called and found that the continent is valid for the game (its innerGraph is connected)

public:
	//accessors...
	int getId() { return *id; };
	string getName() { return *name; };
	int getSize() { return *size; };
	int getWorth() { return *worth; };
	Graph getInnerGraph() { return *innerGraph; };
	bool getValidated() { return *validated; };
	bool getIsValid() { return *isValid; };

	//constructor, copy constructor, destructor, assignment operator
	Continent(int id, string name, int worth);
	Continent(const Continent& old);
	~Continent();
	const Continent& operator=(const Continent& c); //assignment operator

	//methods
	void addCountryToGraph(Country country); //adds a country to the continent's innerGraph, increments continent size. Sets "validated" to false because now the continent has been modified, it needs to be validated again.
	void updateCountryInGraph(Country country); //used if a country's neighbors were modified. Sets "validated" to false because now the continent has been modified, it needs to be validated again.
	bool validate(); //Checks if the continent's "innerGraph" is connected, sets the isValid variable accordingly.
};



//Representation of a map
class Map {
private:
	string* name;
	list<Continent>* continents;
	Graph* graph;  //a graph of the full map. Constructed during execution of the map's validate() method.
	list<Country*>* countries;
	bool* validated;  //true if the map's validate() method has been called.
	bool* isValid;  //true if the map's validate() method has been called and found that the map is valid for the game.

public:
	//accessors... (most of them return copies not pointers to prevent modifications from outside)
	string getName() { return *name; };
	list<Continent> getContinents() { return *continents; };
	Graph getGraph() { return *graph; };
	list<Country*>* getCountries() { return countries; };
	bool getValidated() { return *validated; };
	bool getIsValid() { return *isValid; };

	//constructor, copy constructor, destructor, assignment operator
	Map(string name);
	Map(const Map& old);
	~Map();
	const Map& operator=(const Map& m); //assignment operator

	//methods
	void addCountry(Country country); //adds a country to the Map and to its corresponding Continent's graph. Sets "validated" to false because now the map has been modified, it needs to be validated again.
	void addContinent(Continent country); //adds a continent to the Map. Sets "validated" to false because now the map has been modified, it needs to be validated again.
	void updateContinentsGraphs(); //updates graphs in case a country's neighbors were modified after the country was added to the map.
	bool validate(); //calls each continent's validate() method, proceeds if valid, constructs the map's full graph, checks if it is connected. Sets the isValid variable accordingly.
	Country* getCountryById(int id); //THROWS EXCEPTION if no country was found with this id. Otherwise, returns a pointer to the Country.
	Continent* getContinentById(int id); //THROWS EXCEPTION if no continent was found with this id. Otherwise, returns a pointer to the Continent.
};