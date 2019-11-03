#include "Cards.h"
#include "Map.h"
#include "MapLoader.h"
#include "Player.h"

int main() {

	Deck* deckPtr = new Deck(50);
	Map* map = MapLoader::loadMapFile("mapToTestFortificationPhase");

	/* The loaded map has one continent with the following structure of countries (number of armies in parentheses):

	A(3)---B(1)---C(3)---D(1)---E(1)---F(3)

	Player will be assigned countries A, C, E and F.
	*/

	Player* player = new Player("Andrew", deckPtr, map);
	list<Country*> ownedCountries = list<Country*>();
	
	Country* countryA = map->getCountryById(1);
	Country* countryC = map->getCountryById(3);
	Country* countryE = map->getCountryById(5);
	Country* countryF = map->getCountryById(6);
	ownedCountries.push_back(countryA);  //Country A
	ownedCountries.push_back(countryC);  //Country C
	ownedCountries.push_back(countryE);  //Country E
	ownedCountries.push_back(countryF);  //Country F

	player->setOwnedCountries(ownedCountries);

	countryA->armies = 3;
	countryC->armies = 3;
	countryF->armies = 3;

	player->fortify();
	//the above method will try unsuccesfully to fortify from A, from C and from E. Will successfully fortify from F to E!

	cout << "\n---------------MAP---------------" << endl;
	cout << "Country --> number of armies" << endl;
	for (Country* c : *(map->getCountries()))
		cout << c->name << " --> " << c->armies << endl;
	cout << "---------------------------------" << endl;


	
	return 0;
}