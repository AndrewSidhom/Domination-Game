#include "Map.h"
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <unordered_set>
using namespace std;



//GRAPH CLASS
//Representation of a graph using adjacency lists. Nodes are referred to by ids. Each node maps to a list of its adjacent nodes.

//constructor
Graph::Graph() : adjLists(new map<int, list<int> >()) {}

//destructor
Graph::~Graph() { delete adjLists; };

//copy constructor
Graph::Graph(const Graph& old) { adjLists = new map<int, list<int> >(*old.adjLists); }

//assignment operator
const Graph& Graph::operator=(const Graph& g)
{
	if (&g != this) {
		delete adjLists;
		adjLists = new map<int, list<int> >(*(g.adjLists));
	}
	return *this;
}

//get map of nodes
map<int, list<int>> Graph::getNodes(){
	return *adjLists;
}

//update if node with this id exists, add otherwise.
void Graph::addOrUpdateNode(int id, list<int> neighbors){
	(*adjLists)[id] = neighbors;
}

//checks if the graph is a connected graph using BFS traversal. This code is adapted from the one found at https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/
bool Graph::isConnected()
{
	//Trivial case: if a graph is empty, it is connected
	if (adjLists->empty())
		return true;  
	
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

//constructors
Country::Country() {}
Country::Country(int id, int continentId, string name) : id(id), continentId(continentId), name(name), player(nullptr), armies(-1) {}
Country::Country(int id, int continentId, string name, list<int> neighbors) : id(id), continentId(continentId), name(name), neighbors(neighbors), player(nullptr), armies(-1) {}


//CONTINENT CLASS

//constructor
Continent::Continent(int id, string name, int worth) : id(new int(id)), name(new string(name)), worth(new int(worth)), size(new int(0)), innerGraph(new Graph()), validated(new bool(false)), isValid(new bool(false)) {}

//destructor
Continent::~Continent(){
	delete id; delete name; delete worth, delete innerGraph; delete validated; delete isValid;
}

//copy constructor
Continent::Continent(const Continent& old) {
	id = new int(*old.id);
	name = new string(*old.name);
	worth = new int(*old.worth);
	size = new int(*old.size);
	innerGraph = new Graph(*old.innerGraph);
	validated = new bool(*old.validated);
	isValid = new bool(*old.isValid);
}

//assignment operator
const Continent& Continent::operator=(const Continent& c)
{
	if (&c != this) {
		delete id; delete name; delete worth, delete innerGraph; delete validated; delete isValid;
		id = new int(*c.id);
		name = new string(*c.name);
		worth = new int(*c.worth);
		size = new int(*c.size);
		innerGraph = new Graph(*c.innerGraph);
		validated = new bool(*c.validated);
		isValid = new bool(*c.isValid);
	}
	return *this;
}

//adds a country to the continent's innerGraph, increments continent size. Sets "validated" to false because now the continent has been modified, it needs to be validated again.
void Continent::addCountryToGraph(Country country)
{
	innerGraph->addOrUpdateNode(country.id, country.neighbors);
	(*size)++; 
	*validated = false;
	*isValid = false;
}

//used if a country's neighbors were modified. Sets "validated" to false because now the continent has been modified, it needs to be validated again.
void Continent::updateCountryInGraph(Country country)
{
	innerGraph->addOrUpdateNode(country.id, country.neighbors);
	*validated = false;
	*isValid = false;
}

//Checks if the continent's "innerGraph" is connected, sets the isValid variable accordingly.
bool Continent::validate()
{
	*isValid = innerGraph->isConnected();
	*validated = true;
	return *isValid;
}



//MAP CLASS

//constructor
Map::Map(string name) : name(new string(name)), continents(new list<Continent>()), graph(new Graph()), countries(new list<Country*>()), validated(new bool(false)), isValid(new bool(false)) {}

//destructor
Map::~Map() {
	delete name, continents, countries, graph, validated, isValid;
}

//copy constructor
Map::Map(const Map& old) {
	name = new string(*old.name);
	continents = new list<Continent>(*old.continents);
	graph = new Graph(*old.graph);
	countries = new list<Country*>(*old.countries);
	validated = new bool(*old.validated);
	isValid = new bool(*old.isValid);
}

//assignment operator
const Map& Map::operator=(const Map& m)
{
	if (&m != this) {
		delete name, continents, graph, countries, validated, isValid;
		name = new string(*m.name);
		continents = new list<Continent>(*m.continents);
		graph = new Graph(*m.graph);
		countries = new list<Country*>(*m.countries);
		validated = new bool(*m.validated);
		isValid = new bool(*m.isValid);
	}
	return *this;
}

//adds a country to the Map and to its corresponding Continent's graph. Sets "validated" to false because now the map has been modified, it needs to be validated again.
//THROWS EXCEPTION invalid_argument if the country's continentId is invalid
void Map::addCountry(Country country) {
	try {
		getCountryById(country.id);
	}
	catch (invalid_argument e) {  //if the exception is thrown, that means no country already exists with this id, so proceed to add the country and return.
		countries->push_back(new Country(country));

		getContinentById(country.continentId)->addCountryToGraph(country);

		*validated = false;
		*isValid = false;
		return;
	}
	//exception was not thrown, meaning this id already exists
	cout << "Country with id " << country.id << " was not added to the map because a country with the same id already exists. Cannot add same country twice!";
}

//adds a continent to the Map. Sets "validated" to false because now the map has been modified, it needs to be validated again.
void Map::addContinent(Continent continent) {
	continents->push_back(continent);
	*validated = false;
	*isValid = false;
}

//updates graphs in case a country's neighbors were modified after the country was added to the map.
void Map::updateContinentsGraphs()
{
	for (Country* country : *countries) {
		try {
			getContinentById(country->continentId)->updateCountryInGraph(*country);
		}
		catch (invalid_argument e) {
			cout << "Unable to update country with ID " << country->id << " in continent with ID " << country->continentId << " because this continent ID was not found" << endl;
		}
	}
}

//calls each continent's validate() method, proceeds if valid, constructs the map's full graph, checks if it is connected. Sets the isValid variable accordingly.
bool Map::validate() {
	updateContinentsGraphs();
	if(countries->empty() || continents->empty()) {
			cout << "There are no nodes added within the map." << endl;
			*validated = true;
			*isValid = false;
			return *isValid;
	}
	for(Continent continent : *continents)
		if (!continent.validate()) {
			cout << "Continent with ID " << continent.getId() << " is invalid (its graph is not connected)." << endl;
			*validated = true;
			*isValid = false;
			return *isValid;
		}
	//At this point, all continents were found valid, so construct map's full graph, check if connected.
	for (Country* country : *countries)
		graph->addOrUpdateNode(country->id, country->neighbors);
	*isValid = graph->isConnected();
	*validated = true;
	return *isValid;
}

//THROWS EXCEPTION if no country was found with this id. Otherwise, returns a pointer to the Country.
Country* Map::getCountryById(int id) {
	for (Country* country : *countries) {
		if (country->id == id)
			return country;
	}
	throw invalid_argument("No country found with this ID: " + to_string(id));
}

//THROWS EXCEPTION if no continent was found with this id. Otherwise, returns a pointer to the Continent.
Continent* Map::getContinentById(int id) {
	for (Continent& continent : *continents) {
		if (continent.getId() == id)
			return &continent;
	}
	throw invalid_argument("No continent found with this ID: " + to_string(id));
}