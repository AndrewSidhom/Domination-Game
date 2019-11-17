#include "GameObservers.h"
#include "Player.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

/*************** Subject class *****************/

Subject::Subject() {
	_observers = new list<Observer*>;
}
Subject::~Subject() {
	delete _observers;
}
void Subject::Attach(Observer* o) {
	_observers->push_back(o);
};
void Subject::Detach(Observer* o) {
	_observers->remove(o);
};
void Subject::Notify() {
	list<Observer*>::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i)
		(*i)->Update();
};

/*************** Observer class *****************/

Observer::Observer() {
};
Observer::~Observer() {
};

/*************** StatsObserver class *****************/

StatsObserver::StatsObserver(vector<Player*> subjects) {
	_subjects = new vector<Player*>(subjects);
	mapSize = new int(subjects.at(0)->getNumOfMapCountries());
}

StatsObserver::~StatsObserver() {
	delete _subjects;
	delete mapSize;
}

void StatsObserver::Update() {
	bool removePlayer = false;
	vector<Player*>::iterator it = _subjects->begin();
	while (it != _subjects->end()) {
		if ((*it)->getNumOfOwnedCountries() == 0) {
			removePlayer = true;
			break;
		}
		it++;
	}
	if (removePlayer)
		_subjects->erase(it);
	if (_subjects->size() == 1) { //only one player left means the player has won the game
		cout << endl;
		cout << "CONGRATULATIONS " << _subjects->at(0)->getName() << "!! You have conquered the map and won this game!!" << endl;
	}
	display();
}

void StatsObserver::display() {
	cout << endl;
	cout << "WORLD DOMINATION STATISTICS:" << endl;
	cout << left;
	for (Player* player : *_subjects) {
		float fractionDominated = (float) player->getNumOfOwnedCountries() / *mapSize;
		string dashes = "";
		int i = 1;
		while (i <= floor(fractionDominated * 50)) {
			dashes += "-";
			i++;
		}
		cout << "Player " << player->getId() << " : " << setw(12) << player->getName() << " " << dashes << " ";
		cout << fixed;
		cout << setprecision(2);
		cout << fractionDominated * 100 << "%" << endl;
	}
	cout << endl;

}
/************ PhaseLogObserver class **************/

PhaseLogObserver::PhaseLogObserver(PhaseLog *s) {
	_subject = s;
	_subject->Attach(this);
}

PhaseLogObserver::~PhaseLogObserver() {
	_subject->Detach(this);
}

void PhaseLogObserver::Update() {
	displayMsg();
}

void PhaseLogObserver::displayMsg() {
	cout << endl << *(_subject->getMsg()) << endl;
}