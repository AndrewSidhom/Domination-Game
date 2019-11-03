#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Map.h"


void setUpMapAndPlayers(Map *mapPtr, Player* players) {

    Continent cont1(1, "ZA WARULDO", 10); /// ID, NAME, WORTH
    mapPtr->addContinent(cont1);
    Country c1(1, 1, "DIO");    // ID, CONTINENT ID, NAME
    Country c2(2, 1, "JOJO");
    Country c3(3, 1, "SPEEDWAGON");
    mapPtr->addCountry(c1);
    mapPtr->addCountry(c2);
    mapPtr->addCountry(c3);

    players[0].claimCountry(mapPtr->getCountryById(1), 1);  // COUNTRY* ptr, armies
    players[1].claimCountry(mapPtr->getCountryById(2), 1);
    players[2].claimCountry(mapPtr->getCountryById(3), 1);
}

int main() {

    // Init
    Deck* deckPtr = new Deck(3);
    Map* mapPtr = new Map("Map Game Loop Test");
    int numTotalPlayers = 3;

    // Create players 1, 2, and 3
    Player* players = new Player[numTotalPlayers] {
        Player("Player 1", deckPtr, mapPtr),
        Player("Player 2", deckPtr, mapPtr),
        Player("Player 3", deckPtr, mapPtr)
    };

    // Populate map with country 1, 2, 3, owned by player 1, 2, 3 respectively
    setUpMapAndPlayers(mapPtr, players);
    int numTotalCountries = mapPtr->getCountries()->size();
    
    /*  Start game loop.
    */
    GameEngine ge(players, numTotalPlayers, numTotalCountries);
    ge.startGameLoop();

    return 0;
}