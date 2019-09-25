#pragma once
#include <iostream>
using namespace std;

class Dice {
public:
	Dice();
	~Dice();
	int *roll();

	/*int *rollCounters;
	int *totalRolls;
	double *stats;*/
private:
	int *rollCounters;
	int *totalRolls;
	double *stats;

	int queryDice();
	void update(int container[], int size);
	int *rngAndSort(int numOfDice);
};