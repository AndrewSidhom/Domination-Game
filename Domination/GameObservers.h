#pragma once

#include <list>
#include <vector>
using namespace std;

// Abstract Observer class.
class Observer {
public:
	~Observer();
	virtual void Update() = 0;	// observer updates something
protected:
	Observer();
};

// Subject (Observable) class.
class Subject {
public:
	virtual void Attach(Observer* o); // Attach observer to subject
	virtual void Detach(Observer* o); // Detach observer from subject
	virtual void Notify();	// notify all observers to update
	Subject();
	~Subject();
private:
	list<Observer*>* _observers;
};

// forward declaration for circular dependency
class Player;
class PhaseLog;

class StatsObserver : public Observer {
public:
	StatsObserver(vector<Player*> subjects);
	~StatsObserver();
	void Update();	// update status of players
	void display(); // displays updated statistics of players
private:
	vector<Player*>* _subjects;
	int* mapSize;  //number of countries on the game's map
};

class PhaseLogObserver : public Observer {
public:
	PhaseLogObserver(PhaseLog *_subject);
	~PhaseLogObserver();
	void Update();	// updates status of game
	void displayMsg(); // cout updated logs/messages from the game
private:
	PhaseLog* _subject;
};
