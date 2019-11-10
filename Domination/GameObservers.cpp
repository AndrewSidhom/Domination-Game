#include "GameObservers.h"

/*************** Observable class *****************/

Observable::Observable() {
	_observers = new list<Observer*>;
}
Observable::~Observable() {
	delete _observers;
}
void Observable::Attach(Observer* o) {
	_observers->push_back(o);
};
void Observable::Detach(Observer* o) {
	_observers->remove(o);
};
void Observable::Notify() {
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