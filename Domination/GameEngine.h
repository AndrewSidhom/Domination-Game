#pragma once
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include <vector>
using std::vector;

class GameEngine {

public:
    GameEngine();   // constructor
	GameEngine(Map* map, vector<char> strategies, int maxTurns);  //constructor used by tournaments
	GameEngine(const GameEngine &ge); // copy constructor
	GameEngine& operator=(const GameEngine &ge); // assignment operator
    ~GameEngine();  // destructor
	

	vector<Player*>* getPlayers() { return playerPtrs; }
	int getNumOfPlayers() { return *NUM_OF_PLAYERS; }

    string startGameLoop();   // Start the game loop. Game ends when a player owns all countries. Retruns the name of the player who won or "Draw"

private:

    vector<Player*>* playerPtrs; // dynamic array
	PhaseLog* phaseLog;	// observable obj that notifies change whenever a msg needs to be printed to console
	PhaseLogObserver* phaseLogObserver; // phase log observer;
	Map* gameMap;
	Deck* deck;
    const int *NUM_OF_PLAYERS; // number of total players in the game; do not modify
    const int *NUM_OF_COUNTRIES; // number of total countries in the game; do not modify
	AggressivePlayerStrategy* aggressiveStrategy; // The Agressive computer strategy
	BenevolentPlayerStrategy* benevolentStrategy; // The Benveolent computer strategy
	RandomPlayerStrategy* randomStrategy;
	CheaterPlayerStrategy* cheaterStrategy;
	PlayerStrategy* humanStrategy; // The Human strategy
	int* maxTurns; //in tournament mode, the maximum number of turns to play before the game is declared a draw

	Player* aPlayerOwnsAllCountries(); // Checks if a player owns all countries on the map, returns that player or nullptr.

	void setupObservers();
	void setupStrategies();
	Map* loadGameMap();
	void setupPlayers(Deck *deck, Map *gameMap);
	int queryNumOfPlayers(); //Queries user and validates number of players
	int getStartupArmies();
	void randomizePlayerOrder();
	void assignCountriesToPlayers(Map *gameMap);
	void assignArmiesToCountries();
	void promptChangeStrategy(Player* curPlayer);
};

class Tournament {
public:
	Tournament(); //constructor
	Tournament(const Tournament& old); //copy constructor
	const Tournament& operator=(const Tournament& t);  //assignment operator
	~Tournament();  //destructor
	void setUpWithUserInput();
	void playTournament();
private:
	vector<Map*>* maps;
	vector<char>* playerStrategies; //'a' for agressive, 'b' for benevolent, 'r' for random, 'c' for cheater
	int* gamesPerMap;
	int* maxTurns;
};