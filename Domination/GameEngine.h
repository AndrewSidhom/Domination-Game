#pragma once
#include "Player.h"

class GameEngine {

public:
    GameEngine();   // constructor
    ~GameEngine();  // destructor

    void startGameLoop();   // Start the game loop.

private:

    Player* players; // can be STL containers if needed, otherwise keep as array
    int NUM_OF_PLAYERS; // number of total players in the game; do not modify
    int NUM_OF_COUNTRIES; // number of total countries in the game; do not modify

    bool aPlayerOwnsAllCountries(); // Checks if a player owns all countries on the map.
};