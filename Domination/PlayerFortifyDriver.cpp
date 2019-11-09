//#include "Cards.h"
//#include "Map.h"
//#include "MapLoader.h"
//#include "Player.h"
//
//int main() {
//
//	Deck* deckPtr = new Deck(50);
//	Map* map = MapLoader::loadMapFile("mapToTestFortificationPhase");
//
//	cout << "The loaded map has one continent with the following structure of countries (in parentheses is the # of armies that will be placed):" << endl;
//	cout << "---------------------------------------" << endl;
//	cout << "A(3)---B(1)---C(3)---D(1)---E(1)---F(3)" << endl;
//	cout << "---------------------------------------" << endl;
//	cout << "Player1 will be assigned countries A, C, E and F." << endl << endl;
//
//	Player* player1 = new Player("Andrew", deckPtr, map);
//	list<Country*> ownedCountries = list<Country*>();
//	
//	Country* countryA = map->getCountryById(1);
//	Country* countryC = map->getCountryById(3);
//	Country* countryE = map->getCountryById(5);
//	Country* countryF = map->getCountryById(6);
//	ownedCountries.push_back(countryA);  
//	ownedCountries.push_back(countryC);  
//	ownedCountries.push_back(countryE);  
//	ownedCountries.push_back(countryF); 
//
//	player1->setOwnedCountries(ownedCountries);
//
//	countryA->armies = 3;
//	countryC->armies = 3;
//	countryF->armies = 3;
//
//	Player* player2 = new Player("Other Player", deckPtr, map);
//	list<Country*> ownedCountries2 = list<Country*>();
//
//	Country* countryB = map->getCountryById(2);
//	Country* countryD = map->getCountryById(4);
//	ownedCountries2.push_back(countryB);  
//	ownedCountries2.push_back(countryD);  
//
//	player2->setOwnedCountries(ownedCountries2);
//
//	//the below method will try unsuccesfully to fortify from A, from C and from E. Will successfully fortify from F to E!
//	player1->fortify();
//	
//
//	cout << "\n---------------MAP---------------" << endl;
//	cout << "Country --> number of armies" << endl;
//	for (Country* c : *(map->getCountries()))
//		cout << c->name << " --> " << c->armies << endl;
//	cout << "---------------------------------" << endl;
//
//	system("pause");
//	
//	return 0;
//}