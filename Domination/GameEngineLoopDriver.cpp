//#include "GameEngine.h"
//#include "Player.h"
//#include "Cards.h"
//#include "Map.h"
//
///*
//	The GameEngineLoopDriver tests the game loop in GameEngine class. 
//    Instantiate Map and Players with test data and call startGameLoop() method.
//	Ensure that...
//		1. Each player can call reinforce, attack, and fortify
//		2. Players with no countries who lost cannot play by skipping their turn
//		3. Game loop ends when a player owns all countries on map and wins game
//*/
//
//// Populate map with country 1, 2, 3, owned by player 1, 2, 3 respectively
//void setUpMapAndPlayers(Map *mapPtr, Player* players) {
//
//    Continent cont1(1, "ZA WARULDO", 10); /// ID, NAME, WORTH
//    mapPtr->addContinent(cont1);
//    Country c1(1, 1, "DIO");    // ID, CONTINENT ID, NAME
//    Country c2(2, 1, "JOJO");
//    Country c3(3, 1, "SPEEDWAGON");
//    mapPtr->addCountry(c1);
//    mapPtr->addCountry(c2);
//    mapPtr->addCountry(c3);
//
//    players[0].claimCountry(mapPtr->getCountryById(1), 1);  // COUNTRY* ptr, armies
//    players[1].claimCountry(mapPtr->getCountryById(2), 1);
//    players[2].claimCountry(mapPtr->getCountryById(3), 1);
//}
//
///*  Start driver to test game loop.
//*/
//int main() {
//
//    // Init
//    Deck* deckPtr = new Deck(3);
//    Map* mapPtr = new Map("Map Game Loop Test");
//    int numTotalPlayers = 3;
//
//    // Create players 1, 2, and 3
//    Player* players = new Player[numTotalPlayers] {
//        Player("Player 1", deckPtr, mapPtr),
//        Player("Player 2", deckPtr, mapPtr),
//        Player("Player 3", deckPtr, mapPtr)
//    };
//
//    // Populate map with country 1, 2, 3, owned by player 1, 2, 3 respectively
//    setUpMapAndPlayers(mapPtr, players);
//    int numTotalCountries = mapPtr->getCountries()->size();
//    
//    /*  Start game loop.
//        EXPECT:
//            1. 1st Round: Every player (1-3) will do their turn
//            2. 2nd Round: Player 1 defeats Player 2 and Player 2's turn is skipped
//            3. 3rd Round: Player 1 defeats Player 3
//            4. Immediately, game loop ends and see if conditions apply for Player 1's victory
//    */
//    GameEngine ge(players, numTotalPlayers, numTotalCountries);
//    ge.startGameLoop();
//
//	system("pause");
//
//    return 0;
//}