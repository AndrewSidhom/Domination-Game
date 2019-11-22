#include <iostream>
#include <random>
#include <functional>
#include <string>
#include "Dice.h"
using namespace std;

/**
* Constructor that initializes two arrays of size 6 and sets every element to 0.
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

/**
* Destructor that deallocates rollCounters, totalRolls, and stats.
*/
Dice::~Dice() {
	delete [] rollCounters;
	delete totalRolls;
	delete [] stats;
}

// Copy Constructor
Dice::Dice(const Dice& d) {
	rollCounters = d.rollCounters;
	totalRolls = d.totalRolls;
	stats = d.stats;
}

// Assignment operator =	
Dice& Dice::operator=(const Dice& d) {
	if (&d != this) {
		delete [] rollCounters;
		delete totalRolls;
		delete [] stats;
		totalRolls = new int(*d.totalRolls);
		rollCounters = new int[6];
		for (int i = 0; i < 6; i++) {
			rollCounters[i] = d.rollCounters[i];
		}
		stats = new double[6];
		for (int i = 0; i < 6; i++) {
			stats[i] = d.stats[i];
		}
	}
	return *this;
}

/**
* Asks user how many dice to roll, rolls the appropriate amount of dice, and updates the percentage
* @return dice in a container (sorted in descending order)
*/
list<int> Dice::roll()
{
	int numOfDice = queryDice(); //Asks user how many dice will be rolled
	cout << "Rolling " << numOfDice << " dice..." << endl;
	list<int> container = rngAndSort(numOfDice);
	update(container);
	return container;
}

/**
* Rolls the specified amount of dice and updates the percentage
* @return dice in a container (sorted in descending order)
*/
list<int> Dice::roll(int numberOfDiceToRoll)
{
	list<int> container = rngAndSort(numberOfDiceToRoll);
	update(container);
	return container;
}

/**
* Requests input from the user and validates the input.
* @return the number of dice to be rolled.
*/
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

/**
* Updates number of times a certain number has been rolled and computes the percentage.
*/
void Dice::update(list<int> container)
{
	int index;
	list <int> ::iterator it;
	for (it = container.begin(); it != container.end(); ++it) {
		index = *it;
		rollCounters[index-1] += 1; //Increment value
	}
	*totalRolls += container.size();
	for (int i = 0; i < 6; i++) {
		stats[i] = ((double)rollCounters[i] / *totalRolls) * 100; //Computes percentage of values compared to rolls
	}
}

/*
* Randomly generate numbers between 1 and 6 according to the number of dice selected, stores in a container and sorts it in descending order.
* @param number of dice to be rolled.
* @return container of sorted dice
*  CODE FOR RNG WILL PROBABLY CHANGE
*/
list<int> Dice::rngAndSort(int numOfDice)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, 6);
	list<int> container;
	for (int i = 1; i <= numOfDice; i++) {
		container.push_back(dist(mt));
	}
	container.sort(greater<int>());
	return container;
}

/**
* ONLY USED FOR TESTING
* This is a variant of queryDice. Its only difference is that it allows an input 5000 dice for ease 
* of tests.
* Requests input from the user and validates the input.
* @return the number of dice to be rolled.
*/
int Dice::queryDiceForTests()
{
	string input;
	bool isValid = false;
	do {
		cout << "Enter number of dice you would like to roll (1-3): " << flush;
		cin >> input;
		if (input == "1" || input == "2" || input == "3" || input == "5000")
			isValid = true;
		else
			cout << "Error: Invalid input" << endl;
	} while (!isValid);
	int numOfDice = stoi(input);
	return numOfDice;
}

/**
* Displays the info of total number of rolls, number of individual values rolled, and perecentage of individual values rolled.
* THIS IS USED FOR TESTING PURPOSES
*/
void Dice::showInfo() {
	for (int i = 0; i < 6; i++) {
		cout << "# of " << i + 1 << "'s rolled = " << rollCounters[i] << endl;
	}
	cout << "Total Rolls = " << *(totalRolls) << endl;
	for (int i = 0; i < 6; i++) {
		cout << "% of " << i + 1 << "'s rolled = " << stats[i] << endl;
	}
}

/**
* ONLY USED FOR TESTING
* This is a variant of roll(). Its only difference is that it calls queryDiceForTests() instead of queryDice().
* Asks user how many dice to roll, rolls the apporopriate amount of dice, and updates the percentage
* @return dice in a container
*/
list<int> Dice::rollForTests()
{
	int numOfDice = queryDiceForTests(); //Asks user how many dice will be rolled
	cout << "Rolling " << numOfDice << " dice..." << endl;
	list<int> container = rngAndSort(numOfDice);
	update(container);
	return container;
}
