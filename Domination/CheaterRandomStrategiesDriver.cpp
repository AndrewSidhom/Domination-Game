#include "GameEngine.h"
#include "Player.h"
#include "PlayerStrategies.h"
#include <iostream>
#include <vector>
using namespace std;

// global members for easy access
PhaseLog* phaseLog = new PhaseLog();
PhaseLogObserver* phaseLogObserver = new PhaseLogObserver(phaseLog);
PlayerStrategy* humanStrategy = new PlayerStrategy(phaseLog);
AggressivePlayerStrategy* aggressiveStrategy = new AggressivePlayerStrategy(phaseLog);
BenevolentPlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy(phaseLog);
RandomPlayerStrategy* randomStrategy = new RandomPlayerStrategy(phaseLog);
CheaterPlayerStrategy* cheaterStrategy = new CheaterPlayerStrategy(phaseLog);

// Prompt during test to change (or keep same) strategies before start of each turn.
void promptSetStrategy(Player* player) 
{
    cout << "\nChoose strategies for " + player->getName() + ":";
    cout << "\n\t(0) Human strategy\n\t(1) Aggressive strategy\n\t(2) Benevolent strategy";
    cout << "\n\t(3) Random strategy\n\t(4) Cheater strategy\n";

    int choice;
    cin >> choice;

    switch (choice) {
        case 0: player->setStrategy(humanStrategy); break;
        case 1: player->setStrategy(aggressiveStrategy); break;
        case 2: player->setStrategy(benevolentStrategy); break;
        case 3: player->setStrategy(randomStrategy); break;
        case 4: player->setStrategy(cheaterStrategy); break;
        default: break;
    }
}

/*  Tests the PlayerStrategies class with Cheater and Random strategy as the focus. 
	Ensure that...
		REINFORCE:
		1. Random strategy randomly chooses:
            - to exchange or not.
            - a country to give +2 bonus army from exchanging.
            - any countries to place armies until out of armies.
            - a # of armies to place.
        2. Cheater strategy will cheat by:
            - giving +2 bonus army to all countries matching exchanged cards.
            - double all current armies to all countries
        ATTACK:
		1. Random strategy randomly chooses:
			- a Country to attack
			- a number of time to attack this Country
		2. Cheater strategy will cheat by:
			- automatically conquering all the neighbours of all its Countries
        FORTIFY:
		1. Random strategy randomly chooses:
			- a Country to fortify
		2. Cheater strategy will cheat by:
			- doubling the number of armies on its Countries that have neighbours that belong to other players
*/
int main() {

    GameEngine ge;
    vector<Player*>* playerPtrs = ge.getPlayers();
    int turn = 0, numPlayers = ge.getNumOfPlayers();
    string str;

    do {
        turn = turn % numPlayers;

        // set strategy to test
        promptSetStrategy(playerPtrs->at(turn));
        playerPtrs->at(turn)->getStrategy()->setPlayer(playerPtrs->at(turn));

        // draw 3 cards
        for(int i = 0; i < 3; i++)
        playerPtrs->at(turn)->getHand()->drawFromDeck();

        // call reinforce, attack and fortify
        playerPtrs->at(turn)->reinforce();
		playerPtrs->at(turn)->attack();
		playerPtrs->at(turn)->fortify();
        turn++;
        
        cout << "\nContinue testing? (y/n): "; 
        cin >> str;
        if (str == "n") break;
    } 
    while(true);

    delete phaseLog, phaseLogObserver, humanStrategy, aggressiveStrategy, benevolentStrategy, randomStrategy, cheaterStrategy;
    system("pause");
    return 0;
}