//#include "PlayerStrategies.h"
//#include "Player.h"
//#include "MapLoader.h"
//
//#include <iostream>
//using namespace std;
//
//// Driver goals are:
////	1. Show that different players can be assigned different strategies that lead to different behavior for the reinforcement, attack, and fortification phases using the strategy pattern; 
//// 2. Show that the strategy adopted by a player can be changed dynamically during play;
//// 3. Show that the human player makes decisions according to user interaction, and computer players make decisions automatically, which are both implemented using the strategy pattern.
//
//
//// Show info on Player's Countries (which Countries belongs to them, how many armies are on them, which Countries are neighbors).
//void showPlayerCountriesInfo(Player* p, list<Country*> pCountries, Map* validMap) {
//	cout << "-------------------------" << endl;
//	cout << "Player " << p->getName() << "'s Countries:" << endl;
//
//	for (auto& country : pCountries) {
//		cout << endl << "Country (" << country->id << ") " << country->name << ":" << endl;
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
//		cout << endl << "Country (" << country->id << ") " << country->name << ":" << endl;
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
//
//int main() {
//	// Create Map
//	Map* validMap = MapLoader::loadMapFile("valid");
//	cout << endl;
//
//	// Create Deck
//	Deck* d = new Deck(6);
//
//	// Create PhaseLog
//	PhaseLog* phaseLog = new PhaseLog();
//	PhaseLogObserver* plo = new PhaseLogObserver(phaseLog);
//
//	// Create Players ans PlayerStrategies
//	BenevolentPlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy(phaseLog);
//	CheaterPlayerStrategy* cheaterStrategy = new CheaterPlayerStrategy(phaseLog);
//
//	Player* p1 = new Player("Judy", d, validMap, cheaterStrategy, phaseLog);
//	Player* p2 = new Player("Nick", d, validMap, benevolentStrategy, phaseLog);
//
//	cheaterStrategy->setPlayer(p1);
//	benevolentStrategy->setPlayer(p2);
//
//	// Give Countries to Players
//	list<Country*>* countries = validMap->getCountries();
//	list<Country*> p1Countries = list<Country*>();
//	list<Country*> p2Countries = list<Country*>();
//	int j = 0;
//	for (auto& i : *countries) {
//		if (j < 2) {
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
//	// Set number of armies for Judy and Nick's Countries 
//	for (auto& i : p1Countries) {
//		i->armies = 3;
//	}
//	for (auto& i : p2Countries) {
//		i->armies = 3;
//	}
//
//	// Show the Players' Countries info
//	showPlayerCountriesInfo(p1, p1Countries, validMap);
//	cout << endl;
//	showPlayerCountriesInfo(p2, p2Countries, validMap);
//
//	// Judy's turn
//	//p1->reinforce();
//	p1->attack();
//	p1->fortify();
//
//	// Shows that Map's Countries info after Judy's turn
//	showCountriesInfo(countries, validMap);
//
//	system("pause");
//
//	return 0;
//}