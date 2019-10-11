#pragma once
#include <iostream>
#include <list>
using namespace std;

class Dice {
public:
	Dice();
	~Dice();
	list<int> roll();
	void showInfo(); 
	list<int> rollForTests();

private:
	int *rollCounters;
	int *totalRolls;
	double *stats;

	int queryDice();
	void update(list<int> container);
	list<int> rngAndSort(int numOfDice);
	int queryDiceForTests();
};