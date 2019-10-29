#include "GameEngine.h"

// Constructor
GameEngine::GameEngine() {}
// Destructor
GameEngine::~GameEngine() {}

void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
    do {
        while(players[curPlayerIndex].getNumOfOwnedCountries == 0) 
            { curPlayerIndex ++; }  // skip turn if current player has no countries left

        //players[curPlayerIndex].reinforce();
        //players[curPlayerIndex].attack();
        //players[curPlayerIndex].fortify();

        if(curPlayerIndex == NUM_OF_PLAYERS - 1)
            { curPlayerIndex = 0; }
    } 
    while(!aPlayerOwnsAllCountries());
}

bool GameEngine::aPlayerOwnsAllCountries() {

    for(int i = 0; i < NUM_OF_PLAYERS; i++) {
        if(players[i].getNumOfOwnedCountries() == NUM_OF_COUNTRIES) 
            return true;
    }

    return false;
}