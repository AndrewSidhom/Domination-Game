//#include <iostream>
//#include <map>
//#include <list>
//#include "Player.h"
//#include "Cards.h"
//#include "Map.h"
//using namespace std;
//
///*
//	The PlayerReinforceDriver tests the Player class. Instantiate Player with test data and call reinforce() method.
//	Ensure that...
//		1. Returns correct armies from owned country and continent.
//		2. Armies in countries update accordingly after exchange() is called.
//		3. Exchange() follows the rules of game (i.e. must exchange after 5 cards in hand).
//        4. Allow user to distribute their armies with strong input checking.
//*/
//
///*  Populate map with dummy test data:
//    - 50 TOTAL COUNTRIES
//    - 3 CONTINENTS (3, 20, 17 RESPECTIVELY)
//*/
//void populateMap(Map *mapPtr) {
//
//    Continent cont1(1, "NORTH AMERICA", 10); /// ID, NAME, WORTH
//    Continent cont2(2, "EUROPE", 12);
//    Continent cont3(3, "ASIA", 20);
//    mapPtr->addContinent(cont1);
//    mapPtr->addContinent(cont2);
//    mapPtr->addContinent(cont3);
//    for(int i = 1; i <= 3; i++) {   // TOTAL 3
//        Country c1(i, 1, "NA COUNTRY");    // ID, CONTINENT ID, NAME
//        mapPtr->addCountry(c1);
//    }
//    for(int i = 4; i <= 33; i++) {  // TOTAL 30
//        Country c2(i, 2, "EU COUNTRY");
//        mapPtr->addCountry(c2);
//    }
//    for(int i = 34; i <= 50; i++) { // TOTAL 17
//        Country c3(i, 3, "AS COUNTRY");
//        mapPtr->addCountry(c3);
//    }
//}
//
///* A1. owns 0 continents & own 1 country -> 0.33 -> should get 3 armies
//*/
//void testCaseA1(Player *player, Map *mapPtr) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case A1: own 1 country, 0 continents. Expect total armies: 3\n";  
//    player->claimCountry(mapPtr->getCountryById(4), 1);     // country 4, continent 2 (EU)
//    player->reinforce();
//
//    player->loseCountry(4); // reset
//}
//
///* A2. owns 1 continent & own 4 countries  -> 1.33 -> should get 3 armies
//*/
//void testCaseA2(Player *player, Map *mapPtr) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case A2: own 4 countries(3), 1 continent(NA)(10). Expect total armies: 13\n";
//    player->claimCountry(mapPtr->getCountryById(4), 1);     // country 4, continent 2 (EU)
//    player->claimCountry(mapPtr->getCountryById(1), 1);    // country 1, continent 1 (NA)
//    player->claimCountry(mapPtr->getCountryById(2), 1);    // country 2, continent 1 (NA)
//    player->claimCountry(mapPtr->getCountryById(3), 1);    // country 3, continent 1 (NA)
//    player->reinforce();
//
//    for(int i = 1; i <= 4; i++)
//        player->loseCountry(i); // reset
//}
//
///* A3. owns 2 continents & own 47 countries -> 15.66 -> should get 15 armies
//*/
//void testCaseA3(Player *player, Map *mapPtr) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case A3: own 47 countries(15), 2 continents(12+20). Expect total armies: 47\n";
//    for(int i = 4; i <= 50; i++) {
//        player->claimCountry(mapPtr->getCountryById(i), 1);    // all countries of continent 2 and 3 (EU+AS)
//    }
//    player->reinforce();
//
//    for(int i = 4; i <= 50; i++)
//        player->loseCountry(i); // reset
//}
//
///* A4. receive correct armies from first exchange to 8+ exchange
//*/
//void testCaseA4(Player *player, Map* mapPtr, Deck *deckPtr) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case A4: ensure every exchange gives correct army amount.\n";
//    player->disableArmyDistribution = true; // skip distributing because tested by A1-A3
//
//    int correctVals[8] {4, 6, 8, 10, 12, 15, 20, 25};
//    for(int i = 1; i <= 8; i++) 
//    {
//        cout << "\nExchange " << i << ": expect " << correctVals[i-1] << " armies from exchange\n";
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//        player->reinforce();
//    }    
//}
//
///*  B1. have < 3 cards. Expect: no option to exchange.
//*/
//void testCaseB1(Player *player, Deck *deck) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case B1: have < 3 cards. Expect: no option to exchange.\n";
//    player->getHand()->addCardToHand(deck->draw());
//    player->getHand()->addCardToHand(deck->draw());
//    cout << "\nTotal cards in hand: " << player->getHand()->getHandCount() << endl;
//
//    player->reinforce();
//}
//
///*  B2. have 3 cards. Expect: no option to exchange or cancel.
//*/
//void testCaseB2(Player *player, Deck *deck) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case B2: have >= 3 cards. Expect: option to exchange or cancel.\n";
//    player->setHand(deck);  /// reset Hand
//    player->getHand()->addCardToHand(Card(1, INFANTRY));
//    player->getHand()->addCardToHand(Card(1, INFANTRY));
//    player->getHand()->addCardToHand(Card(1, INFANTRY));
//    cout << "\nTotal cards in hand: " << player->getHand()->getHandCount() << endl;
//
//    player->reinforce();
//}
//
///*  B3. have 5 cards. Expect: mandatory exchange.
//*/
//void testCaseB3(Player *player, Deck *deck) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case B3: have 5 cards. Expect: mandatory exchange.\n";
//    player->setHand(deck);  /// reset Hand
//    for(int i = 0; i < 5; i++) {
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//    }
//    cout << "\nTotal cards in hand: " << player->getHand()->getHandCount() << endl;
//
//    player->reinforce();
//}
//
///*  B4. have 6 cards. Expect: mandatory exchange THEN optional because have 3 cards later.
//*/
//void testCaseB4(Player *player, Deck *deck) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case B4: have 6 cards. Expect: mandatory exchange THEN optional because have 3 cards later.\n";
//    player->setHand(deck);  /// reset Hand
//    for(int i = 0; i < 6; i++) {
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//    }
//    cout << "\nTotal cards in hand: " << player->getHand()->getHandCount() << endl;
//
//    player->reinforce();
//}
//
///*  B5. player defeats another player and takes their cards, possessing 10 cards total. 
//        Expect: mandatory exchange until 4 cards or less.
//*/
//void testCaseB5(Player *player, Deck *deck) {
//
//    cout << "-----------------------------------------\n"
//        << "Test Case B5: have 10 cards. Expect: mandatory exchange until < 4 cards.\n";
//    player->setHand(deck);  /// reset Hand
//    for(int i = 0; i < 10; i++) {
//        player->getHand()->addCardToHand(Card(1, INFANTRY));
//    }
//    cout << "\nTotal cards in hand: " << player->getHand()->getHandCount() << endl;
//
//    player->reinforce();
//}
//
///*  Main func to start all test methods.
//    Inputs to consider checking when distributing armies:
//    - if player doesnt own country
//    - if armies input is < 0 or > armies given
//    - if input isnt number
//*/
//int main() {
//
//    /// INIT
//    Deck* deckPtr = new Deck(50);
//    Map* mapPtr = new Map("Map Reinforce Test");
//    Player* player = new Player("Justin", deckPtr, mapPtr);
//    /// POPULATE
//    populateMap(mapPtr);
//
//    /*  TEST # OF ARMY REINFORCEMENTS FROM..
//        CONTINENTS              |COUNTRIES
//        A1. owns 0 continents   |A1. own 1 country    -> 0.33 -> should get 3 armies
//        A2. owns 1 continent    |A2. own 4 countries  -> 1.33 -> should get 3 armies
//        A3. owns 2 continents   |A3. own 47 countries -> 15.66-> should get 15 armies
//        EXCHANGING
//        A4. receive correct armies from first exchange to 8+ exchange
//    */
//    testCaseA1(player, mapPtr);
//    testCaseA2(player, mapPtr);
//    testCaseA3(player, mapPtr);
//    testCaseA4(player, mapPtr, deckPtr);
//
//    /*  TEST EXCHANGE DEPENDING ON CARDS IN HAND
//        NOTE: run 
//        B1. have < 3 cards. Expect: no option to exchange.
//        B2. have 3 cards. Expect: no option to exchange.
//        B3. have 5 cards. Expect: mandatory exchange.
//        B4. have 6 cards. Expect: mandatory exchange THEN optional because have 3 cards later.
//        B5. player defeats another player and takes their cards, possessing 10 cards total. 
//            Expect: mandatory exchange until 4 cards or less.
//    */
//    player->disableArmyDistribution = true;
//    testCaseB1(player, deckPtr);
//    testCaseB2(player, deckPtr);
//    testCaseB3(player, deckPtr);
//    testCaseB4(player, deckPtr);
//    testCaseB5(player, deckPtr);
//
//    return 0;
//}