#include "GameObservers.h"
#include "Player.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

/*************** Observer class *****************/
// Abstract Constructor
Observer::Observer() {};
// Abstract Destructor
Observer::~Observer() {};

/*************** Subject class *****************/
// Constructor
Subject::Subject() {
	_observers = new list<Observer*>;
}
// Destructor
Subject::~Subject() {
	delete _observers;
}
// Attach observer to subject by adding to observer list
void Subject::Attach(Observer* o) {
	_observers->push_back(o);
};
// Detach observer to subject by removing from observer list
void Subject::Detach(Observer* o) {
	_observers->remove(o);
};
// Iterate through observer list and call update on each observers
void Subject::Notify() {
	list<Observer*>::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i)
		(*i)->Update();
};

/*************** StatsObserver class *****************/
// Constructor
StatsObserver::StatsObserver(vector<Player*> subjects) {
	_subjects = new vector<Player*>(subjects);
	
	//register this observer with each Player subject
	for (Player* pl : *_subjects)
		pl->Attach(this);
	
	mapSize = new int(subjects.at(0)->getNumOfMapCountries());
}
// Destructor
StatsObserver::~StatsObserver() {
	for (Player* pl : *_subjects)
		pl->Detach(this);
	delete _subjects;
	delete mapSize;
}

void StatsObserver::Update() {
	//check if there is a player who is no longer playing (i.e. owns zero countries). If there is, set removePlayer to true
	bool removePlayer = false;
	vector<Player*>::iterator it = _subjects->begin();
	while (it != _subjects->end()) {
		if ((*it)->getNumOfOwnedCountries() == 0) {
			removePlayer = true;
			break; //Note: we break as soon as a player to remove is found because everytime the observer is notified, there is at most 1 such player
		}
		it++;
	}
	//if removePlayer is true, use the current iterator value (which was iterating through Player subjects) to remove that Player from subjects
	if (removePlayer)
		_subjects->erase(it);

	//check if there is only one player left, i.e. the player has won the game
	if (_subjects->size() == 1) { 
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
		while (i <= floor(fractionDominated * 50)) { //this loop adds dashes proportional to how much of the world the player dominates, for a visual representation of domination.
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
// Constructor for observer. Calls subject to attach itself to subject.
PhaseLogObserver::PhaseLogObserver(PhaseLog *s) {
	_subject = s;
	_subject->Attach(this);
}
// Destructor for observer. Calls subject to detaches itself from subject.
PhaseLogObserver::~PhaseLogObserver() {
	_subject->Detach(this);
}
// Updates log when game is updated
void PhaseLogObserver::Update() {
	displayMsg();
}
// Print out the message given from game to console.
void PhaseLogObserver::displayMsg() {
	cout << *(_subject->getMsg()) << endl;
}