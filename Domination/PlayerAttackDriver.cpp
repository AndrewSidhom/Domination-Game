//#include "Player.h"
//#include "MapLoader.h"
//
//#include <iostream>
//using namespace std;
//
//// Driver goals:
//	// 1. Show that only valid attacks can be declared (i.e. valid attacker/attacked country)
//	// 2. Show that only valid number of dice can be chosen by the attacker/defender
//	// 3. Show that given known dice values, that the right number of armies are deducted on the attacker/defender
//	// 4. Show that the attacker is allowed to initiate multiple attacks, until it declares that it does not want to 
//	//    attack anymore.
//
//
//// Show info on Player's Countries (which Countries belongs to them, how many armies are on them, which Countries are neighbors).
//void showPlayerCountriesInfo(Player* p, list<Country*> pCountries, Map* validMap) {
//	cout << "-------------------------" << endl;
//	cout << "Player " << p->getName() << "'s Countries:" << endl;
//
//	for (auto& country : pCountries) {
//		cout << endl << "Country " << country->name << ":" << endl;
//		cout << "\tArmies: " << country->armies << endl;
//
//		cout << "\tNeighbouring Countries:";
//		bool noEnemyNeighbor = true;
//		list<int> neighbors = country->neighbors;
//		for (auto& neighbor : neighbors) {
//			cout << " " << validMap->getCountryById(neighbor)->name << " ";
//
//			if (validMap->getCountryById(neighbor)->player->getId() != p->getId()) {
//				noEnemyNeighbor = false;
//			}
//		}
//		cout << endl;
//
//		if (country->armies >= 2) {
//			if (noEnemyNeighbor) {
//				cout << "\tThis Country cannot attack other Countries because it has no enemy neighbor" << endl;
//			}
//			else {
//				cout << "\tThis Country can attack Countries:";
//				for (auto& neighbor : neighbors) {
//					if (validMap->getCountryById(neighbor)->player->getId() != p->getId()) {
//						cout << " " << validMap->getCountryById(neighbor)->name << " ";
//					}
//				}
//				cout << endl;
//			}
//		}
//		else {
//			cout << "\tThis Country cannot attack other Countries because it has less than two armies." << endl;
//		}
//
//		cout << endl;
//	}
//	cout << "-------------------------" << endl;
//}
//
//// Show info of a list of Countries (who this Country belongs to, number of armies on it, neighbors)
//void showCountriesInfo(list<Country*>* countries, Map* validMap) {
//	cout << "-------------------------" << endl;
//	cout << "Countries info:" << endl;
//
//	for (auto& country : *countries) {
//		cout << endl << "Country " << country->name << ":" << endl;
//		cout << "\tPlayer owner: " << country->player->getName() << endl;
//		cout << "\tArmies: " << country->armies << endl;
//
//		cout << "\tNeighbouring Countries:";
//		list<int> neighbors = country->neighbors;
//		for (auto& neighbor : neighbors) {
//			cout << " " << validMap->getCountryById(neighbor)->name << " ";
//		}
//		cout << endl;
//	}
//	cout << "-------------------------" << endl;
//}
//
//int main() {
//	// Create Map
//	Map* validMap = MapLoader::loadMapFile("valid");
//	cout << endl;
//
//	// Create Deck
//	Deck* d = new Deck(6);
//
//	// Create Players
//	Player* p1 = new Player("Judy", d, validMap);
//	Player* p2 = new Player("Nick", d, validMap);
//
//	// Give Countries to Players
//	list<Country*>* countries = validMap->getCountries();
//	list<Country*> p1Countries = list<Country*>();
//	list<Country*> p2Countries = list<Country*>();
//	int j = 0;
//	for (auto& i : *countries) {
//		if (j < 3) {
//			i->player = p1;
//			p1Countries.push_back(i);
//		}
//		else {
//			i->player = p2;
//			p2Countries.push_back(i);
//		}
//
//		j++;
//	}
//	p1->setOwnedCountries(p1Countries);
//	p2->setOwnedCountries(p2Countries);
//
//	// Set number of armies for Judy's Countries 
//	// (Nick's Countries will have the default number of armies, i.e. 1)
//	for (auto& i : p1Countries) {
//		i->armies = 3;
//	}
//
//	// Show the Players' Countries info
//	showPlayerCountriesInfo(p1, p1Countries, validMap);
//	cout << endl;
//	showPlayerCountriesInfo(p2, p2Countries, validMap);
//
//	// Player Judy will attack
//	cout << endl << "/////////////////////////////" << endl;
//	cout << "PLAYER JUDY WILL ATTACK" << endl;
//	cout << "/////////////////////////////" << endl;
//
//	p1->attack();
//
//	cout << "/////////////////////////////" << endl;
//	cout << "END OF PLAYER JUDY'S ATTACK" << endl;
//	cout << "/////////////////////////////" << endl << endl;
//
//	// Shows that Map's Countries info after Judy's attack
//	showCountriesInfo(countries, validMap);
//
//	// Player Nick will try to attack
//	cout << endl << "/////////////////////////////" << endl;
//	cout << "PLAYER NICK WILL ATTACK" << endl;
//	cout << "/////////////////////////////" << endl;
//
//	p2->attack();
//
//	cout << "/////////////////////////////" << endl;
//	cout << "END OF PLAYER NICK'S ATTACK" << endl;
//	cout << "/////////////////////////////" << endl << endl;
//
//	// Shows that Map's Countries info after Nick's attack
//	showCountriesInfo(countries, validMap);
//
//	system("pause");
//
//	return 0;
//}
