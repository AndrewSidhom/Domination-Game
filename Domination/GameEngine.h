#pragma once
#include "Player.h"
#include "Map.h"
#include "PhaseLog.h"
#include <random>

class GameEngine {

public:
    GameEngine();   // constructor
    ~GameEngine();  // destructor

    void startGameLoop();   // Start the game loop. Game ends when a player owns all countries. 

private:

    Player* players; // dynamic array
	PhaseLog* phaseLog;	// observable obj that notifies change whenever a msg needs to be printed to console
    const int *NUM_OF_PLAYERS; // number of total players in the game; do not modify
    const int *NUM_OF_COUNTRIES; // number of total countries in the game; do not modify

	bool aPlayerOwnsAllCountries(); // Checks if a player owns all countries on the map.

	void setupObservers();
	Map* loadGameMap();
	int queryNumOfPlayers(); //Queries user and validates number of players
	void setupPlayers(Deck *deck, Map *gameMap);
	int getStartupArmies();
	void randomizePlayerOrder();
	void assignCountriesToPlayers(Map *gameMap);
	void assignArmiesToCountries();
	
};