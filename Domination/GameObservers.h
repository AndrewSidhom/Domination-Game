#pragma once

#include <list>
#include <vector>
using namespace std;

//abstract
class Observer {
public:
	~Observer();
	virtual void Update() = 0;
protected:
	Observer();
};

class Subject {
public:
	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify();
	Subject();
	~Subject();
private:
	list<Observer*>* _observers;
};


class Player; //this is a forward declaration, needed because StatsObserver will be using a pointer to Player
class PhaseLog;

class StatsObserver : public Observer {
public:
	StatsObserver(vector<Player*> subjects);
	~StatsObserver();
	void Update();
	void display();
private:
	vector<Player*>* _subjects;
	int* mapSize;  //number of countries on the game's map
};

class PhaseLogObserver : public Observer {
public:
	PhaseLogObserver(PhaseLog *_subject);
	~PhaseLogObserver();
	void Update();
	void displayMsg();
private:
	PhaseLog* _subject;
};
