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
//	AggressivePlayerStrategy* aggressiveStrategy = new AggressivePlayerStrategy();
//	BenevolentPlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy();
//	PlayerStrategy* humanStrategy = new PlayerStrategy();
//
//	Player* p1 = new Player("Judy", d, validMap, aggressiveStrategy, phaseLog);
//	Player* p2 = new Player("Nick", d, validMap, benevolentStrategy, phaseLog);
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
//	// Play for two turns
//	for (int i = 1; i < 3; i++) {
//		cout << "\n/////////////////" << endl;
//		cout << "TURN " << i << endl;
//		cout << "/////////////////" << endl;
//
//		cout << "\nChoose one of the following strategies for Player Judy:" << endl;
//		cout << "\t(0) Agressive strategy" << endl;
//		cout << "\t(1) Benevolent strategy" << endl;
//		cout << "\t(2) Human strategy" << endl;
//
//		int choice = -1;
//		cin >> choice;
//
//		while (!cin.good() || (choice != 0 && choice != 1 && choice != 2)) {
//			cout << endl << "This input is wrong. Please enter 0, 1 or 2.";
//			cin >> choice;
//		}
//
//		cout << endl;
//
//		switch (choice) {
//		case 0:
//			p1->setStrategy(aggressiveStrategy);
//			break;
//		case 1:
//			p1->setStrategy(benevolentStrategy);
//			break;
//		case 2:
//			p1->setStrategy(humanStrategy);
//			break;
//		}
//
//		cout << "\nChoose one of the following strategies for Player Nick:" << endl;
//		cout << "\t(0) Agressive strategy" << endl;
//		cout << "\t(1) Benevolent strategy" << endl;
//		cout << "\t(2) Human strategy" << endl;
//		
//		choice = -1;
//		cin >> choice;
//
//		while (!cin.good() || (choice != 0 && choice != 1 && choice != 2)) {
//			cout << endl << "This input is wrong. Please enter 0, 1 or 2.";
//			cin >> choice;
//		}
//
//		cout << endl;
//
//		switch (choice) {
//		case 0:
//			p2->setStrategy(aggressiveStrategy);
//			break;
//		case 1:
//			p2->setStrategy(benevolentStrategy);
//			break;
//		case 2:
//			p2->setStrategy(humanStrategy);
//			break;
//		}
//
//		// Judy's turn
//		p1->reinforce();
//		p1->attack();
//		p1->fortify();
//
//		// Shows that Map's Countries info after Judy's turn
//		showCountriesInfo(countries, validMap);
//
//		// Nick's turn
//		p2->reinforce();
//		p2->attack();
//		p2->fortify();
//
//		// Shows that Map's Countries info after Nick's turn
//		showCountriesInfo(countries, validMap);
//	}
//
//	system("pause");
//
//	return 0;
//}