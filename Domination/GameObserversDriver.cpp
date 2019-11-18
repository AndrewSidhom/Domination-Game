#include "GameObservers.h"
#include "GameEngine.h"
#include "Player.h"
#include <vector>

int main() {
	GameEngine ge;
	int numOfPlayers = ge.getNumOfPlayers();
	Player* playerPtr = ge.getPlayers();
	
	vector<Player*> playersVec;
	for (int i = 0; i < numOfPlayers; i++) {
		playersVec.push_back(playerPtr);
		playerPtr++;
	}
	
	//initialize the Observer by passing to it its subjects
	StatsObserver observer (playersVec);

	ge.startGameLoop();

	return 0;
}