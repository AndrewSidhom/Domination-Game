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

		// Get all adjacent nodes of the dequeued node id. If an adjacent node has not been visited, then mark it visited and enqueue it 
		for (int a : (*adjLists)[id]){
			if (visited.count(a)==0){
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


