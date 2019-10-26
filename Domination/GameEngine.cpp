#include "GameEngine.h"

// Constructor
GameEngine::GameEngine() {}
// Destructor
GameEngine::~GameEngine() {}

void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
    do {
        while(players[curPlayerIndex].getOwnedArmies().size() == 0) // TODO change armies to countries method after refector
            { curPlayerIndex ++; }  // skip turn if lost player has no countries left

        //players[curPlayerIndex].reinforce();
        //players[curPlayerIndex].attack();
        //players[curPlayerIndex].fortify();
    } 
    while(!aPlayerOwnsAllCountries());
}

bool GameEngine::aPlayerOwnsAllCountries() {

    bool bPlayerOwnsAll = false;

    for(int i = 0; i < NUM_OF_PLAYERS; i++) {
        if(players[i].getOwnedArmies().size() == NUM_OF_COUNTIES) // TODO change armies to countries method after refector
            { bPlayerOwnsAll = true; }
    }

    return bPlayerOwnsAll;
}