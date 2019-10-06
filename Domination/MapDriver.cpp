#include <iostream>
#include <string>
#include <list>
#include "Map.h"
using namespace std;

/** Class responsible for the testing of class Map.
 */ 
int main() {

    cout << "Creating valid map.." << endl;
    Map m1 = createMap("Test Map 1");

    cout << "\nValidating if map is a connected graph and "
            << "continents are connected subgraphs.. ";
    if(m1.validate())
        cout << "VALID" << endl;
    else
        cout << "INVALID" << endl;
    
    showGraphAsList(m1);

    Map m2("Test Map 2");

    //the map is a connected graph
    //continents are connected subgraphs
    //each country belongs to one and only one continent
    //The driver must provide test cases for various valid/invalid maps
}

/*  Create a map.
    @param name of map.
*/
Map createMap(string name) {
    
    Map map(name);
    int numOfConts, numOfCountries, countryIndex = 1;
    
    cout << "# of continents: ";
    cin >> numOfConts;

    for(int i = 1; i <= numOfConts; i++)
    {
        Continent cont(i, ("CONTINENT "+i), i++);
        map.addContinent(cont);

        cout << endl << "# of countries for " << cont.getName() << ": ";
        cin >> numOfCountries;

        if (numOfCountries == 1) {
            Country c(countryIndex, cont.getId(), ("COUNTRY "+countryIndex));
            countryIndex++;

            map.addCountry(c);
        }
        else {
            int start = countryIndex;
            int end = countryIndex + numOfCountries;

            for(int j = 1; j <= numOfCountries; j++)
            {
                list<int> neighbors;
                for(int s = start; s <= end; s++) {
                    if(s != countryIndex)
                        neighbors.push_back(s);
                }

                Country c(countryIndex, cont.getId(), ("COUNTRY "+countryIndex), neighbors);
                countryIndex++;

                map.addCountry(c);
            }
        }
    }

    return map;
}

/*  Output if valid or not.
*/
void showGraphAsList(Map m) {

    cout << "\nGraph as list:\n-------------\n";
    cout << "Sort by Continent:\n\n";

    for(Continent cont : m.getContinents) 
    {
        cout << cont.getName << ": "
    }
}