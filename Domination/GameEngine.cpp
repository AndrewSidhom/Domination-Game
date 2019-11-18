#include "GameEngine.h"
#include "MapLoader.h"
#include "Cards.h"
#include "GameObservers.h"
#include "PlayerStrategies.h"
#include <string>
#include <vector>
using std::cout;
using std::cin;
using std::endl;


// Constructor
GameEngine::GameEngine() {

	// Inits
	setupObservers();
	phaseLog->printMsg("Welcome to Domination!");
	
	// Set up map and deck
	Map *gameMap = loadGameMap();
	Deck deck(*NUM_OF_COUNTRIES);

	// Create players
	setupPlayers(&deck, gameMap);
	randomizePlayerOrder();
	phaseLog->printMsg("Order of player's turn\n-------------------");
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {	
		phaseLog->printMsg(players[i].getName());
	}	
	
	// Assign all countries to players
	assignCountriesToPlayers(gameMap);
	assignArmiesToCountries();	

	aggressiveStrategy = new AggressivePlayerStrategy();
	benevolentStrategy = new BenevolentPlayerStrategy();
	humanStrategy = new PlayerStrategy();
}

// Destructor
GameEngine::~GameEngine() {
	delete [] players;
	players = nullptr;
	delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
	delete aggressiveStrategy;
	delete benevolentStrategy;
	delete humanStrategy;
}

/*	Responsible for starting the game loop. Loop ends when a player owns all countries on map.
	Every loop, each player that owns at least 1 country will call reinforce, attack, fortify in that order.
*/
void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
	Player* curPlayerPtr;
    do {
		curPlayerPtr = &players[curPlayerIndex];
        while(curPlayerPtr->getNumOfOwnedCountries() == 0) { 
			curPlayerIndex++;  // skip turn if current player has no countries left
			curPlayerPtr = &players[curPlayerIndex];
		}
		promptChangeStrategy(curPlayerPtr);
		curPlayerPtr->getStrategy()->setPlayer(curPlayerPtr); // appoint different player to specific strategy every time

		curPlayerPtr->reinforce();
		curPlayerPtr->attack();
		curPlayerPtr->fortify();
		
        if(curPlayerIndex == *NUM_OF_PLAYERS - 1)
            curPlayerIndex = 0;
		else
			curPlayerIndex++;
    } 
    while(!aPlayerOwnsAllCountries());
}

/*	Checks if a player owns all countries on the map.
	@return if a player owns all countries
*/
bool GameEngine::aPlayerOwnsAllCountries() {

    for(int i = 0; i < *NUM_OF_PLAYERS; i++) {
        if(players[i].getNumOfOwnedCountries() == *NUM_OF_COUNTRIES) 
            return true;
    }
    return false;
}

/*	Sets up PhaseLog (subject) with PhaseLogObserver to display msg related to changes in phases.
*/
void GameEngine::setupObservers() {
	phaseLog = new PhaseLog();
	PhaseLogObserver* plo = new PhaseLogObserver(phaseLog);
}

/* 	Asks input for the map to be use in the game. Also stores number of total countries on map.
	@return a pointer to a Map object that corresponds to the loaded map.
*/
Map* GameEngine::loadGameMap() {
	string mapName;
	int numOfCountries;	

	phaseLog->printMsg("Enter the name of the map you would like to load (without the .map extension): ");
	cin >> mapName;
	Map *gameMap = MapLoader::loadMapFile(mapName);
	while (gameMap == nullptr) {
		phaseLog->printMsg("Invalid map name, try again: ");
		cin >> mapName;
		gameMap = MapLoader::loadMapFile(mapName);
	}
	list<Country*> *countries = gameMap->getCountries();
	numOfCountries = (int)countries->size();
	NUM_OF_COUNTRIES = new int(numOfCountries);
	phaseLog->printMsg("Map successfully loaded!");

	return gameMap;
}

// Creates Player objects and prompts for player names
void GameEngine::setupPlayers(Deck *deck, Map *gameMap) {
	// determine number of players and AIs
	int numOfPlayers = queryNumOfPlayers();
	int numOfAIs = 0;
	if(numOfPlayers < 6)
		numOfAIs = queryNumOfAIs(numOfPlayers);
	NUM_OF_PLAYERS = new int(numOfPlayers + numOfAIs);

	// instantiate strategies
	humanStrategy = new PlayerStrategy();
	aggressiveStrategy = new AggressivePlayerStrategy();
	benevolentStrategy = new BenevolentPlayerStrategy();

	// create player objects
	players = new Player[*NUM_OF_PLAYERS];
	for (int i = 0; i < numOfPlayers; i++) {
		string name = "Player " + to_string(i + 1);
		players[i] = Player(name, deck, gameMap, humanStrategy, phaseLog); 

		phaseLog->printMsg((players + i)->getName() + ", enter your new name, or enter '0' to keep your current name: ");
		cin >> name;
		if (name != "0") 
			(players + i)->setName(name);
	}
	// create AI objects
	for (int i = numOfPlayers; i < *NUM_OF_PLAYERS; i++) {
		string name = "AI Player " + to_string(i + 1);
		// alternate between different AIs
		if(i % 2 == 0)	
			players[i] = Player(name, deck, gameMap, aggressiveStrategy, phaseLog); 
		else			
			players[i] = Player(name, deck, gameMap, benevolentStrategy, phaseLog); 
	}
}

/* 	Asks input for the number of Players.
	@returns the number of players.
*/
int GameEngine::queryNumOfPlayers() {
	string input;
	bool isValid = false;
	do {
		phaseLog->printMsg("How many players will be playing? (2-6 players): ");
		cin >> input;
		if(input == "2" || input == "3" || input == "4" || input == "5" || input == "6")
			isValid = true;
		else
			phaseLog->printMsg("Error: Invalid input");
				
	} while (!isValid);
	int numOfPlayers = stoi(input);
	return numOfPlayers;
}

/* 	Asks input for the number of AIs depending on how many humans are playing
	@returns the number of AIs.
*/
int GameEngine::queryNumOfAIs(int numHumanPlayers) {
	string input;
	int spotsLeft = 6 - numHumanPlayers;
	int numOfAIs = 0;
	do {
		phaseLog->printMsg("You have " + to_string(numHumanPlayers) + " players out of 6.");
		phaseLog->printMsg("How many AIs players will join the conquest? (0 to " + to_string(spotsLeft) + "): ");
		cin >> input;
		if(input == "0" || input == "1" || input == "2" || input == "3" || input == "4") {
			numOfAIs = stoi(input);
			if(numOfAIs <= spotsLeft)
				break;
			else
				phaseLog->printMsg("There cannot be more than 6 players.");
		}
		else
			phaseLog->printMsg("Error: Invalid input");
				
	} while (true);
	
	return numOfAIs;
}

// Determines how many armies should be distributed amongst the Players in the startup phase depending on
// the total number of Players.
int GameEngine::getStartupArmies() {
	switch (*NUM_OF_PLAYERS) {
	case 2:
		return 40; break;
	case 3:
		return 35; break;
	case 4:
		return 30; break;
	case 5:
		return 25; break;
	case 6:
		return 20; break;
	}
	
}

// Changes randomly the order of the Players stored in players
void GameEngine::randomizePlayerOrder() {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, *NUM_OF_PLAYERS - 1);

	for (int i = 0; i < *NUM_OF_PLAYERS; i++)
	{
		int rnd = dist(mt);
		Player *temp = new Player(players[i]);
		players[i] = players[rnd];
		players[rnd] = *temp;
	}
}

// Assigns Countries to each Player. Asks input repeatedly and one at a time to each Player to choose a Country to add
// to their list of owned Countries.
void GameEngine::assignCountriesToPlayers(Map *gameMap) {

	int rnd;
	random_device rd;
	mt19937 mt(rd());

	vector<int> countryIds;
	for (int i = 1; i <= *NUM_OF_COUNTRIES; i++) {
		countryIds.push_back(i);
	}
	list<Country*> ownedCountries[6];
	int playerIndex = 0;

	// loop each player (round-robin) and assign it a random country from map
	for (int i = 0; i < *NUM_OF_COUNTRIES; i++) {
		// get a random country id
		uniform_int_distribution<int> dist(0, countryIds.size() - 1);
		rnd = dist(mt);
		// store that randomly selected country to each player
		Country* country = gameMap->getCountryById(countryIds.at(rnd));
		ownedCountries[playerIndex].push_back(country);
		countryIds.erase(countryIds.begin() + rnd);

		playerIndex = (playerIndex + 1) % *NUM_OF_PLAYERS;
	}
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		players[i].setOwnedCountries(ownedCountries[i]);
	}
}

// Assigns armies to each Country.
void GameEngine::assignArmiesToCountries() {

	for (int i = 0; i < *NUM_OF_PLAYERS; i++) 
	{
		phaseLog->printMsg(players[i].getName() + "'s turn: ");
		// note setOwnedCountries already place 1 army in each owned countries to be identified as claimed
		int remainingArmies = getStartupArmies() - players[i].getNumOfOwnedCountries();
		players[i].distributeArmies(remainingArmies);
	}
}

void GameEngine::promptChangeStrategy(Player* curPlayer) {

	phaseLog->printMsg("Would you like to change " + curPlayer->getName() + "'s current strategy? (y/n): ");
	string input;
	do {
		cin >> input;
		if(input.compare("y") == 0 || input.compare("n") == 0)  // 0 means equal
			break;
		else
			cout << "\nInput must be 'y' or 'n'\n";
	} 
	while (true);

	if (input.compare("y") == 0) {
		phaseLog->printMsg("\nChoose one of the following strategies for " + curPlayer->getName() + ":");
		phaseLog->printMsg("\t(0) Aggressive strategy\n\t(1) Benevolent strategy\n\t(2) Human strategy");

		int choice = -1;
		cin >> choice;
		while (!cin.good() || (choice != 0 && choice != 1 && choice != 2)) {
			phaseLog->printMsg("This input is wrong. Please enter 0, 1 or 2.");
			cin >> choice;
		}

		switch (choice) {
		case 0:
			curPlayer->setStrategy(aggressiveStrategy);
			break;
		case 1:
			curPlayer->setStrategy(benevolentStrategy);
			break;
		case 2:
			curPlayer->setStrategy(humanStrategy);
			break;
		}
	}
}