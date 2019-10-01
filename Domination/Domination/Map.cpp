#include <map>
#include <list>
#include <unordered_set>
#include <string>
#include <iostream>
#include "Map.h"
using namespace std;



//GRAPH CLASS

//constructor
Graph::Graph(map<int, list<int> >* adjLists) : adjLists(adjLists) {}

//checks if the graph is a connected graph using BFS traversal
bool Graph::isConnected()
{
	unordered_set<int> visited;
	list<int> queue;
	
	// Get id of the first node of the graph, mark it as visited and enqueue it 
	int id = adjLists->begin()->first;
	visited.insert(id);
	queue.push_back(id);

	while (!queue.empty()){
		// Dequeue a node from queue
		id = queue.front();
		queue.pop_front();

		// Get all adjacent nodes of the dequeued node id. If an adjacent node has not been visited (and belongs to the graph), then mark it visited and enqueue it 
		for (int a : (*adjLists)[id]){
			if (visited.count(a)==0 && adjLists->count(a)==1){
				visited.insert(a);
				queue.push_back(a);
			}
		}
	}
	
	//Conclusion: the graph is not connected if the algorithm has visited less nodes than are in the graph
	if (visited.size() < adjLists->size())
		return false;
	else
		return true;
}



//COUNTRY STRUCTURE

//constructor
Country::Country(int id, int continentId, string name, list<int> neighbors) : id(id), continentId(continentId), name(name), neighbors(neighbors) {}



//CONTINENT CLASS

//constructor
Continent::Continent(int id, string name, int worth) : id(new int(id)), name(new string(name)), worth(new int(worth)), countries(new list<Country*>()), innerGraph(nullptr), validated(new bool(false)), isValid(new bool(false)) {}

//destructor
Continent::~Continent(){
	delete id; delete name; delete countries; delete innerGraph; delete validated; delete isValid;
}

//adds a country to the continent. Note: Country will NOT be added if continent has already been validated and found valid, at which point its list of countries is final.
void Continent::addCountry(Country* country)
{
	if (!validated || !isValid)
		countries->push_back(country);
}

//Constructs the continent's "innerGraph", checks if it is connected, sets the isValid variable accordingly.
bool Continent::validate()
{
	map<int, list<int> >* adjLists = new map<int, list<int> >();
	for (Country* countryPtr : *countries)
		(*adjLists)[countryPtr->id] = countryPtr->neighbors;
	innerGraph = new Graph(adjLists);
	*validated = true;
	*isValid = innerGraph->isConnected();
	return *isValid;
}



//MAP CLASS

//constructor
Map::Map(int id, string name, list<Continent*>* continents) : id(new int(id)), name(new string(name)), continents(continents), graph(nullptr), countries(new list<Country*>()), validated(new bool(false)), isValid(new bool(false)) {}

//destructor
Map::~Map() {
	delete id; 
	delete name;
	for (Continent* continentPtr : *continents)
		delete continentPtr;
	delete continents;
	for (Country* countryPtr : *countries)
		delete countryPtr;
	delete countries; 
	delete graph; 
	delete validated; 
	delete isValid;
}

//calls each continent's validate() method, proceeds if valid, constructs the map's full graph, checks if it is connected. Sets the isValid variable accordingly.
bool Map::validate() {
	for(Continent* continent : *continents)
		if (!continent->validate()) {
			cout << "Continent with ID " << continent->getId() << "is invalid.";
			*validated = true;
			*isValid = false;
			return *isValid;
		}
	//At this point, all continents were found valid, so construct map's full graph, check if connected.
	map<int, list<int> >* adjLists = new map<int, list<int> >();
	for (Country* countryPtr : *countries)
		(*adjLists)[countryPtr->id] = countryPtr->neighbors;
	graph = new Graph(adjLists);
	*validated = true;
	*isValid = graph->isConnected();
	return *isValid;
}

//returns pointer to the country having this id. nullptr is returned if no country was found with this id.
Country* Map::getCountryById(int id) {
	for (Country* country : *countries) {
		if (country->id == id)
			return country;
	}
	return nullptr;
}

//returns pointer to the continent having this id. nullptr is returned if no continent was found with this id.
Continent* Map::getContinentById(int id) {
	for (Continent* continent : *continents) {
		if (continent->getId == id)
			return continent;
	}
	return nullptr;
}

//adds a country to the Map and to its corresponding Continent. Note: Country will NOT be added if map has already been validated and found valid, at which point its list of countries is final.
void Map::addCountry(Country* country){
	if (!validated || !isValid) {
		countries->push_back(country);
		Continent* continent = getContinentById(country->continentId);
		continent->addCountry(country);
	}

}