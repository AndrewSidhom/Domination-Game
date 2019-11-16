#include "GameObservers.h"
#include "PhaseLog.h"
#include "Player.h"
#include <iostream>
#include <string>
using std::cout;

/*************** Observable class *****************/

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

StatsObserver::StatsObserver(vector<Player*> observables) {
	_observables = new vector<Player*>(observables);
}

StatsObserver::~StatsObserver() {
	delete _observables;
}

void StatsObserver::Update() {

}

/*************** StatsObserver class *****************/

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