#pragma once
#include "Player.h"
#include "Map.h"
#include <random>

class GameEngine {

public:
    GameEngine();   // constructor
    ~GameEngine();  // destructor
    // TODO REMOVE AFTER PHASE 2 TESTING. Constructor used to init test data.
    GameEngine(Player *testPlayers, int totalPlayers, int totalCountries);   

    void startGameLoop();   // Start the game loop. Game ends when a player owns all countries. 

private:

    Player* players; // dynamic array
    const int *NUM_OF_PLAYERS; // number of total players in the game; do not modify
    const int *NUM_OF_COUNTRIES; // number of total countries in the game; do not modify

    bool aPlayerOwnsAllCountries(); // Checks if a player owns all countries on the map.
	int queryNumOfPlayers(string name); //Queries user and validates number of players
	Map* loadGameMap();
	void randomOrder();
	void setupPlayers(string name, Deck *deck, Map *gameMap);
	void assignCountriesToPlayers(Map *gameMap);
};