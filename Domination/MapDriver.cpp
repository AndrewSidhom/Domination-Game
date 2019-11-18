//#include <iostream>
//#include <string>
//#include <list>
//#include <map>
//#include "Map.h"
//using namespace std;
//
///*
//	The MapDriver takes care of testing the Map class.
//
//		1. It requests number of continents and number of countries per continent input from the user 
//		to generate a custom valid map. The MapDriver then presents all the nodes in that map to prove 
//		that the graph is indeed connected and hence that the map is valid.
//
//		2. It tests hard-coded invalid maps by asserting their validity (the Map class checks if these maps are valid
//		and returns false).
//*/
//
///*  Create a map.
//    @param name of map.
//*/
//Map createMap(string name) {
//    
//    Map map(name);
//    int numOfConts, numOfCountries, countryIndex = 1;
//    list<int> allCountriesId;
//    
//    cout << "# of continents: ";
//    cin >> numOfConts;
//
//    for(int i = 1; i <= numOfConts; i++)
//    {
//        Continent cont(i, "CONTINENT", 0);
//        map.addContinent(cont);
//
//        cout << "# of countries for CONTINENT " << cont.getId() << ": ";
//        cin >> numOfCountries;
//
//        for(int j = 1; j <= numOfCountries; j++)
//        {
//            Country c(countryIndex, cont.getId(), ("COUNTRY"));
//            countryIndex++;
//
//            map.addCountry(c);
//            allCountriesId.push_back(c.id);
//        }
//    }
//
//    // make every country have every other countries as neighbors
//    for(Country* c : *map.getCountries()) {
//        list<int> neighbors;
//        for(int id : allCountriesId) {
//            // make sure it doesn't add itself as its own neighbor
//            if(id != c->id) neighbors.push_back(id);
//        }
//        map.getCountryById(c->id)->neighbors = neighbors;
//    }
//
//    return map;
//}
//
///*  Validate if continent's subgraph are connected.
//*/
//void testContinentSubgraph(Map &m) {
//
//    for(Continent cont : m.getContinents()) 
//    {
//        cout << "Continent " << cont.getId() << ": ";
//        if(cont.validate())
//            cout << "VALID\n";
//        else
//            cout << "INVALID\n";
//    }
//}
//
///*  Validate if map's graph is connected.
//*/
//void testMapgraph(Map &m) {
//
//    cout << "Validating '" << m.getName() << "': ";
//    m.validate();
//    if(m.getValidated() && m.getIsValid())
//        cout << "VALID\n";
//    else
//        cout << "INVALID\n";
//}
//
///*  Show a readable list representation of a graph.
//*/
//void showGraphAsList(Map &m) {
//
//    for(Continent cont : m.getContinents()) 
//    {
//        cout << cont.getName() << " (id: " << cont.getId() << ")\n";
//        map<int, list<int>> countryNodes = cont.getInnerGraph().getNodes();
//
//        // loop through subgraph (continent)
//        map<int, list<int>>::iterator countryIter;
//        for(countryIter = countryNodes.begin(); countryIter != countryNodes.end(); ++countryIter) 
//        {    
//            cout << "country id: " << countryIter->first 
//                << ", continent: " << m.getCountryById(countryIter->first)->continentId
//                << ", neighbors: { ";
//
//            // loop through neighbors of this country
//            list<int>::iterator neighborIter = countryIter->second.begin();
//            for(int i = 0, size = countryIter->second.size(); i < size; i++) 
//            {
//                cout << *neighborIter << " ";
//                neighborIter++;
//            }
//
//            cout << "}\n";
//        }
//        cout << "---\n";
//    }
//}
//
///*  Test invalid map: Map without nodes
//    Expect invalid results.
// */ 
//void testMapWithNoNodes() {
//
//    Map map("Test: Map without nodes");
//    testMapgraph(map);
//}
//
///*  Test invalid map: Isolated Continent (not connected)
//    Expect invalid results.
// */ 
//void testIsolatedContinent() {
//
//    Map map("Test: Isolated Continent (not connected)");
//
//    Continent cont1(1, "VALID CONTINENT", 0);
//    Continent cont2(2, "VALID CONTINENT", 0);
//    Continent cont3(3, "ISOLATED CONTINENT", 0);
//    Country c1(1, 1, "COUNTRY", list<int>{2});
//    Country c2(2, 1, "COUNTRY", list<int>{1, 3});
//    Country c3(3, 2, "COUNTRY", list<int>{2});
//    Country c4(4, 3, "COUNTRY", list<int>{5});
//    Country c5(5, 3, "Country", list<int>{4});
//    map.addContinent(cont1);
//    map.addContinent(cont2);
//    map.addContinent(cont3);
//    map.addCountry(c1);
//    map.addCountry(c2);
//    map.addCountry(c3);
//    map.addCountry(c4);
//    map.addCountry(c5);
//
//    testMapgraph(map);
//}
//
///*  Test invalid map: Country with no path with its continent's countries.
//    Expect invalid results.
// */ 
//void testCountryWithBadPathing() {
//
//    Map map("Test: Country with no path with its continent's countries");
//
//    Continent cont1(1, "CONTINENT", 0);
//    Continent cont2(2, "CONTINENT", 0);
//    Country c1(1, 1, "COUNTRY", list<int>{2});
//    Country c2(2, 1, "COUNTRY", list<int>{1,4});
//    Country c3(3, 1, "COUNTRY", list<int>{4});
//    Country c4(4, 2, "COUNTRY", list<int>{5});
//    Country c5(5, 2, "Country", list<int>{4});
//    map.addContinent(cont1);
//    map.addContinent(cont2);
//    map.addCountry(c1);
//    map.addCountry(c2);
//    map.addCountry(c3);
//    map.addCountry(c4);
//    map.addCountry(c5);
//
//    testMapgraph(map);
//}
//
///** Class responsible for the testing of class Map.
// */ 
//int main() {
//
//	// Request input for a number of continents and a number of countries
//	// Generate a valid map from this data and validate the map 
//    cout << "Create a valid map:" << endl;
//    Map m1 = createMap("Test Map 1");
//
//    cout << "\nValidating if map is a connected graph..\n";
//    testMapgraph(m1);
//
//    cout << "\nValidating if continents are connected subgraphs..\n";
//    testContinentSubgraph(m1);
//
//    cout << "\nShow that each country has only one continent:\n ------\n";
//    showGraphAsList(m1);
//
//	// Test for specific hard-coded invalid maps
//    cout << "\nTest for invalid maps:\n";
//    testMapWithNoNodes();
//    testIsolatedContinent();
//    testCountryWithBadPathing();
//    
//	system("pause");
//}