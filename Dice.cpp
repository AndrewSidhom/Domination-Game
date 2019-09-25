#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <functional>
#include <string>
#include "Dice.h"
using namespace std;

/**
* Initializes two arrays of size 6 and sets every element to 0.
* Initializes totalRolls to 0.
*/
Dice::Dice() {
	rollCounters = new int[6];
	for (int i = 0; i < 6; i++) {
		rollCounters[i] = 0;
	}

	totalRolls = new int(0);

	stats = new double[6];
	for (int i = 0; i < 6; i++) {
		stats[i] = 0.0;
	}
}

Dice::~Dice() {
	delete rollCounters, totalRolls, stats;
}

int * Dice::roll()
{
	int numOfDice = queryDice(); //Asks user how many dice will be rolled
	cout << "Rolling " << numOfDice << " dice..." << endl; 
	int *container = rngAndSort(numOfDice);
	update(container, numOfDice);
	return container;
}

int Dice::queryDice() {
	string input;
	bool isValid = false;
	do {
		cout << "Enter number of dice you would like to roll (1-3): " << flush;
		cin >> input;
		if (input == "1" || input == "2" || input == "3")
			isValid = true;
		else
			cout << "Error: Invalid input" << endl;
	} while (!isValid);
	int numOfDice = stoi(input);
	return numOfDice;
}

void Dice::update(int container[], int size) {
	int index;
	for (int i = 1; i <= size; i++) {
		index = container[i]; //Index corresponds to value rolled
		rollCounters[index] += 1; //Increment value
	}
	*totalRolls += size;
	for (int i = 0; i < 6; i++) {
		stats[i] = (double)rollCounters[i] / *totalRolls; //Computes percentage of values compared to rolls
	}
}

/*
*  CODE FOR RNG WILL PROBABLY CHANGE
*/
int * Dice::rngAndSort(int numOfDice)
{
	srand(time(NULL)); //Used to generate different sequences
	int *container = new int[numOfDice+1]; //Create container of size numOfDice + 1 to store size as head 
	container[0] = 6*numOfDice;
	for (int i = 1; i <= numOfDice; i++) {
		container[i] = rand() % 6; // Generates random numbers 0-5 (code will probably change with next lab)
	}
	sort(container, container + numOfDice + 1, greater<int>()); //Sorts array in descending order (can change depending on specifications)
	return container;
}
