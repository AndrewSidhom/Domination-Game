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
Country::Country(int id, string name, list<int> neighbors) : id(id), name(name), neighbors(neighbors) {}


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
