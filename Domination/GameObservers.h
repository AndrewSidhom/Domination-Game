#pragma once

#include <list>
#include <vector>
using namespace std;

class Observable {
public:
	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify();
	Observable();
	~Observable();
private:
	list<Observer*>* _observers;

};
	
//abstract
class Observer {
public:
	~Observer();
	virtual void Update() = 0;
protected:
	Observer();
};


class Player; //this is a forward declaration, needed because StatsObserver will be using a pointer to Player

class StatsObserver : public Observer {
public:
	StatsObserver(vector<Player*> observables);
	~StatsObserver();
	void Update();
private:
	vector<Player*>* _observables;
};
