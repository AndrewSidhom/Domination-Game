#include <iostream>
#include <string>
#include <list>
#include <map>
#include "Map.h"
using namespace std;

/*  Create a map.
    @param name of map.
*/
Map createMap(string name) {
    
    Map map(name);
    int numOfConts, numOfCountries, countryIndex = 1;
    list<int> allCountriesId;
    
    cout << "# of continents: ";
    cin >> numOfConts;

    for(int i = 1; i <= numOfConts; i++)
    {
        Continent cont(i, "CONTINENT", 0);
        map.addContinent(cont);

        cout << "# of countries for CONTINENT " << cont.getId() << ": ";
        cin >> numOfCountries;

        for(int j = 1; j <= numOfCountries; j++)
        {
            Country c(countryIndex, cont.getId(), ("COUNTRY"));
            countryIndex++;

            map.addCountry(c);
            allCountriesId.push_back(c.id);
        }
    }

    // make every country have every other countries as neighbors
    for(Country c : map.getCountries()) {
        list<int> neighbors;
        for(int id : allCountriesId) {
            // make sure it doesn't add itself as its own neighbor
            if(id != c.id) neighbors.push_back(id);
        }
        map.getCountryById(c.id)->neighbors = neighbors;
    }

    return map;
}

/*  Validate if continent's subgraph are connected.
*/
void testContinentSubgraph(Map &m) {

    for(Continent cont : m.getContinents()) 
    {
        cout << "Continent " << cont.getId() << ": ";
        if(cont.validate())
            cout << "VALID\n";
        else
            cout << "INVALID\n";
    }
}

/*  Validate if map's graph is connected.
*/
void testMapgraph(Map &m) {

    cout << "Validating '" << m.getName() << "': ";
    m.validate();
    if(m.getValidated() && m.getIsValid())
        cout << "VALID\n";
    else
        cout << "INVALID\n";
}

/*  Show a readable list representation of a graph.
*/
void showGraphAsList(Map &m) {

    for(Continent cont : m.getContinents()) 
    {
        cout << cont.getName() << " (id: " << cont.getId() << ")\n";
        map<int, list<int>> countryNodes = cont.getInnerGraph().getNodes();

        // loop through subgraph (continent)
        map<int, list<int>>::iterator countryIter;
        for(countryIter = countryNodes.begin(); countryIter != countryNodes.end(); ++countryIter) 
        {    
            cout << "country id: " << countryIter->first 
                << ", continent: " << m.getCountryById(countryIter->first)->continentId
                << ", neighbors: { ";

            // loop through neighbors of this country
            list<int>::iterator neighborIter = countryIter->second.begin();
            for(int i = 0, size = countryIter->second.size(); i < size; i++) 
            {
                cout << *neighborIter << " ";
                neighborIter++;
            }

            cout << "}\n";
        }
        cout << "---\n";
    }
}

/** Class responsible for the testing of class Map.
 */ 
int main() {

    cout << "Create a valid map:" << endl;
    Map m1 = createMap("Test Map 1");

    cout << "Validating if continents are connected subgraphs..\n";
    testContinentSubgraph(m1);

    cout << "\nValidating if map is a connected graph..\n";
    testMapgraph(m1);

    cout << "\nShow that each country has only one continent:\n ------\n";
    showGraphAsList(m1);

    //cout << "Test for invalid maps:";
    //Map m2 = createMap("Test Map 2");
    //The driver must provide test cases for various valid/invalid maps
}