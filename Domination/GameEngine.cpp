#include "GameEngine.h"
#include "MapLoader.h"
#include "GameObservers.h"
#include "PlayerStrategies.h"
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
using namespace std;


// Constructor
GameEngine::GameEngine() {

	// Inits
	setupObservers();
	setupStrategies();
	phaseLog->printMsg("Welcome to Domination!");
	
	// Set up map and deck
	gameMap = loadGameMap();
	deck = new Deck(*NUM_OF_COUNTRIES);

	// Create players
	setupPlayers(deck, gameMap);
	randomizePlayerOrder();
	phaseLog->printMsg("Order of player's turn\n-------------------");
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {	
		phaseLog->printMsg(playerPtrs->at(i)->getName());
	}	
	
	// Assign all countries to players
	assignCountriesToPlayers(gameMap);
	assignArmiesToCountries();

	maxTurns = new int(1000000); //when not in tournament mode, up to a million turns are allowed
}

//constructor used by tournaments
GameEngine::GameEngine(Map* map, vector<char> strategies, int maxTurns)
{	
	// Inits
	setupObservers();
	setupStrategies();

	// Set up map and deck
	gameMap = map;
	list<Country*>* countries = gameMap->getCountries();
	int numOfCountries = (int)countries->size();
	NUM_OF_COUNTRIES = new int(numOfCountries);
	deck = new Deck(*NUM_OF_COUNTRIES);

	// Create players
	NUM_OF_PLAYERS = new int(strategies.size());
	playerPtrs = new vector<Player*>();
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		Player* playerPtr;
		switch (strategies.at(i))
		{
		case 'a': 
			playerPtr = new Player("Agressive", deck, gameMap, aggressiveStrategy, phaseLog); break;
		case 'b':
			playerPtr = new Player("Benevolent", deck, gameMap, benevolentStrategy, phaseLog); break;
		case 'r':
			playerPtr = new Player("Random", deck, gameMap, randomStrategy, phaseLog); break;
		case 'c':
			playerPtr = new Player("Cheater", deck, gameMap, cheaterStrategy, phaseLog); break;
		default:
			break;
		}
		playerPtrs->push_back(playerPtr);
	}
	randomizePlayerOrder();
	phaseLog->printMsg("Order of player's turn\n-------------------");
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		phaseLog->printMsg(playerPtrs->at(i)->getName());
	}

	// Assign all countries to players
	assignCountriesToPlayers(gameMap);
	assignArmiesToCountries();

	this->maxTurns = new int(maxTurns);
}

// Copy constructor
GameEngine::GameEngine(const GameEngine &ge)
{
	playerPtrs = new vector<Player*>();
	for(Player* ptr : *ge.playerPtrs)
		playerPtrs->push_back(ptr);
	phaseLog = new PhaseLog(*ge.phaseLog);
	phaseLogObserver = new PhaseLogObserver(phaseLog);
	gameMap = new Map(*ge.gameMap);
	deck = new Deck(*ge.deck);
	NUM_OF_COUNTRIES = new int(*ge.NUM_OF_COUNTRIES);
	NUM_OF_PLAYERS = new int(*ge.NUM_OF_PLAYERS);
	aggressiveStrategy = new AggressivePlayerStrategy(*ge.aggressiveStrategy);
	benevolentStrategy = new BenevolentPlayerStrategy(*ge.benevolentStrategy);
	humanStrategy = new PlayerStrategy(*ge.humanStrategy);
	randomStrategy = new RandomPlayerStrategy(*ge.randomStrategy);
	cheaterStrategy = new CheaterPlayerStrategy(*ge.cheaterStrategy);
	maxTurns = new int(*ge.maxTurns);
}
	
// Assignment operator
GameEngine& GameEngine::operator=(const GameEngine &ge)
{
	if(&ge != this) {
		delete playerPtrs, phaseLog, phaseLogObserver, gameMap, deck;
		delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
		delete aggressiveStrategy, benevolentStrategy, humanStrategy, randomStrategy, cheaterStrategy;
		delete maxTurns;
		playerPtrs = new vector<Player*>();
		for(Player* ptr : *ge.playerPtrs)
			playerPtrs->push_back(ptr);
		phaseLog = new PhaseLog(*ge.phaseLog);
		phaseLogObserver = new PhaseLogObserver(phaseLog);
		gameMap = new Map(*ge.gameMap);
		deck = new Deck(*ge.deck);
		NUM_OF_COUNTRIES = new int(*ge.NUM_OF_COUNTRIES);
		NUM_OF_PLAYERS = new int(*ge.NUM_OF_PLAYERS);
		aggressiveStrategy = new AggressivePlayerStrategy(*ge.aggressiveStrategy);
		benevolentStrategy = new BenevolentPlayerStrategy(*ge.benevolentStrategy);
		humanStrategy = new PlayerStrategy(*ge.humanStrategy);
		randomStrategy = new RandomPlayerStrategy(*ge.randomStrategy);
		cheaterStrategy = new CheaterPlayerStrategy(*ge.cheaterStrategy);
		maxTurns = new int(*ge.maxTurns);
	}
	return *this;
}

// Destructor
GameEngine::~GameEngine() {
	delete playerPtrs, phaseLog, phaseLogObserver, gameMap, deck;
	playerPtrs = nullptr;
	delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
	delete aggressiveStrategy, benevolentStrategy, humanStrategy, randomStrategy, cheaterStrategy;
	delete maxTurns;
}

/*	Responsible for starting the game loop. Loop ends when a player owns all countries on map.
	Every loop, each player that owns at least 1 country will call reinforce, attack, fortify in that order.
	Returns the name of the player who won or "Draw"
*/
string GameEngine::startGameLoop() {
    
	int turnsPlayed = 0;
	int turn = 0; // index of current player's turn
	Player* winningPlayer = nullptr;  //no winning player in the beginning
    do {
        while(playerPtrs->at(turn)->getNumOfOwnedCountries() == 0) 
			{ turn++; }

		//promptChangeStrategy(playerPtrs->at(turn));
		playerPtrs->at(turn)->getStrategy()->setPlayer(playerPtrs->at(turn)); // appoint different player to specific strategy every time

		playerPtrs->at(turn)->reinforce();
		playerPtrs->at(turn)->attack();
		playerPtrs->at(turn)->fortify();
		
        if(turn == *NUM_OF_PLAYERS - 1)
            turn = 0;
		else
			turn++;
		turnsPlayed++;
		winningPlayer = aPlayerOwnsAllCountries(); //this value remains nullptr if there isn't a player who owns all countries
    } 
    while(!winningPlayer && turnsPlayed < *maxTurns);

	if (!winningPlayer)
		return "Draw";
	else
		return winningPlayer->getName();
}

/*	Checks if a player owns all countries on the map.
	@return the player who owns all countries if there is one or nullptr if not.
*/
Player* GameEngine::aPlayerOwnsAllCountries() {

    for(int i = 0; i < *NUM_OF_PLAYERS; i++) {
		Player* pl = playerPtrs->at(i);
        if(pl->getNumOfOwnedCountries() == *NUM_OF_COUNTRIES) 
            return pl;
    }
    return nullptr;
}

/*	Sets up PhaseLog (subject) with PhaseLogObserver to display msg related to changes in phases.
*/
void GameEngine::setupObservers() {
	phaseLog = new PhaseLog();
	phaseLogObserver = new PhaseLogObserver(phaseLog);
}

// instantiate strategies
void GameEngine::setupStrategies() {
	humanStrategy = new PlayerStrategy(phaseLog);
	aggressiveStrategy = new AggressivePlayerStrategy(phaseLog);
	benevolentStrategy = new BenevolentPlayerStrategy(phaseLog);
	randomStrategy = new RandomPlayerStrategy(phaseLog);
	cheaterStrategy = new CheaterPlayerStrategy(phaseLog);
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
	NUM_OF_PLAYERS = new int(numOfPlayers);

	// create player objects
	playerPtrs = new vector<Player*>();
	for (int i = 0; i < numOfPlayers; i++) {
		string name = "Player " + to_string(i + 1);
		Player* playerPtr = new Player(name, deck, gameMap, humanStrategy, phaseLog); 
		phaseLog->printMsg(playerPtr->getName() + ", enter your new name, or enter '0' to keep your current name: ");
		cin >> name;
		if (name != "0") 
			playerPtr->setName(name);
		playerPtrs->push_back(playerPtr);
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
	default: return 0;
	}
}

// Changes randomly the order of the Players stored in players
void GameEngine::randomizePlayerOrder() {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, *NUM_OF_PLAYERS - 1);

	for (int i = 0; i < *NUM_OF_PLAYERS - 1; i++)
	{
		int rnd = dist(mt);
		if (rnd != i) {
			
			Player* tempPtr = playerPtrs->at(i);
			playerPtrs->at(i) = playerPtrs->at(rnd);
			playerPtrs->at(rnd) = tempPtr;
		}
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
		playerPtrs->at(i)->setOwnedCountries(ownedCountries[i]);
	}
}

// Assigns armies to each Country.
void GameEngine::assignArmiesToCountries() {

	for (int i = 0; i < *NUM_OF_PLAYERS; i++) 
	{
		phaseLog->printMsg("\n" + playerPtrs->at(i)->getName() + "'s turn: ");
		// note setOwnedCountries already place 1 army in each owned countries to be identified as claimed
		int remainingArmies = getStartupArmies() - playerPtrs->at(i)->getNumOfOwnedCountries();
		playerPtrs->at(i)->getStrategy()->setPlayer(playerPtrs->at(i));
		playerPtrs->at(i)->getStrategy()->distributeArmies(remainingArmies);
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
			phaseLog->printMsg("\nInput must be 'y' or 'n'\n");
	} 
	while (true);

	if (input.compare("y") == 0) {
		phaseLog->printMsg("\nChoose one of the following strategies for " + curPlayer->getName() + ":");
		phaseLog->printMsg("\t(0) Human strategy\n\t(1) Aggressive strategy\n\t(2) Benevolent strategy");
		phaseLog->printMsg("\t(3) Random strategy\n\t(4) Cheater strategy");

		int choice = -1;
		cin >> choice;
		while (!cin.good() || (choice != 0 && choice != 1 && choice != 2 && choice != 3 && choice != 4)) {
			phaseLog->printMsg("This input is wrong. Please enter 0, 1, 2, 3, or 4.");
			cin >> choice;
		}

		switch (choice) {
			case 0: curPlayer->setStrategy(humanStrategy); break;
			case 1: curPlayer->setStrategy(aggressiveStrategy); break;
			case 2: curPlayer->setStrategy(benevolentStrategy); break;
			case 3: curPlayer->setStrategy(randomStrategy); break;
			case 4: curPlayer->setStrategy(cheaterStrategy); break;
			default:break;
		}
	}
}





//constructor
Tournament::Tournament() : maps(new vector<Map*>()), playerStrategies(new vector<char>()), gamesPerMap(new int(0)), maxTurns(new int (0)) {}

//copy constructor
Tournament::Tournament(const Tournament& old)
{
	maps = new vector<Map*>(*old.maps);
	playerStrategies = new vector<char>(*old.playerStrategies);
	gamesPerMap = new int(*old.gamesPerMap);
	maxTurns = new int(*old.maxTurns);
}

//assignment operator
const Tournament& Tournament::operator=(const Tournament& t)
{
	if (&t != this) {
		maps = new vector<Map*>(*t.maps);
		playerStrategies = new vector<char>(*t.playerStrategies);
		gamesPerMap = new int(*t.gamesPerMap);
		maxTurns = new int(*t.maxTurns);
	}
	return *this;
}

//destructor
Tournament::~Tournament() { delete maps; delete playerStrategies; delete gamesPerMap; delete maxTurns; }

//Takes input from the user to set up the tournament to be played. Gives values to the data members.
void Tournament::setUpWithUserInput() {
	cout << "Welcome to Tournament mode!" << endl;

	//set up the tournament's maps
	cout << "Enter the number of maps you would like to load (1 to 5)" << endl;
	int numOfMaps;
	cin >> numOfMaps;
	while (!cin.good() || numOfMaps < 1 || numOfMaps > 5) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Incorrent input. Enter a number from 1 to 5." << endl;
		cin >> numOfMaps;
	}
	delete maps;
	maps = new vector<Map*>();
	string mapName;
	for (int i = 1; i <= numOfMaps; i++) {
		cout << "Map " << i << ": Enter the name of the map you would like to load (without the .map extension): " << endl;
		cin >> mapName;
		Map* gameMap = MapLoader::loadMapFile(mapName);
		while (gameMap == nullptr) {
			cout << "Invalid map name, try again: " << endl;
			cin >> mapName;
			gameMap = MapLoader::loadMapFile(mapName);
		}
		maps->push_back(gameMap);
	}

	//set up the tournament's player strategies
	cout << "Enter the number of computer players who will play the tournament (2 to 4)" << endl;
	int numOfPlayers;
	cin >> numOfPlayers;
	while (!cin.good() || numOfPlayers < 2 || numOfPlayers > 4) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Incorrent input. Enter a number from 2 to 4." << endl;
		cin >> numOfPlayers;
	}
	delete playerStrategies;
	playerStrategies = new vector<char>();
	cout << "Choose one of the following strategies by entering its number, for each computer player as prompted below." << endl;
	cout << "(1) Aggressive strategy" << endl; 
	cout << "(2) Benevolent strategy" << endl;
	cout << "(3) Random strategy" << endl; 
	cout << "(4) Cheater strategy" << endl;
	cout << endl;
	for (int i = 1; i <= numOfPlayers; i++) {
		cout << "Strategy for Computer Player " << i << ": " << endl;
		int choice = -1;
		cin >> choice;
		while (!cin.good() || choice < 1 || choice > 4) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Incorrent input. Enter a number from 1 to 4 to indicate the strategy for Player " << i << endl;
			cin >> choice;
		}
		switch (choice) {
			case 1: playerStrategies->push_back('a'); break;
			case 2: playerStrategies->push_back('b'); break;
			case 3: playerStrategies->push_back('r'); break;
			case 4: playerStrategies->push_back('c'); break;
			default:break;
		}
	}

	//set up the number of games per map
	cout << "Enter the number of games you would like to be played on each map (1 to 5)" << endl;
	int games;
	cin >> games;
	while (!cin.good() || games < 1 || games > 5) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Incorrent input. Enter a number from 1 to 5." << endl;
		cin >> games;
	}
	*gamesPerMap = games;

	//set up the maximum number of turns
	cout << "Enter the number of turns per game to be played before the game is declared to be a draw (10 to 50)" << endl;
	int turns;
	cin >> turns;
	while (!cin.good() || turns < 10 || turns > 50) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Incorrent input. Enter a number from 10 to 50." << endl;
		cin >> turns;
	}
	*maxTurns = turns;
}

//plays the tournament to the end and prints the results in a table
void Tournament::playTournament()
{
	vector<string> winners; //will store the winners' name or "Draw", in the order the games were played.

	//loop through games, storing the winner each time
	for (int m = 0; m < maps->size(); m++) {
		for (int g = 0; g < *gamesPerMap; g++) {
			GameEngine ge(maps->at(m), *playerStrategies, *maxTurns);
			string winner = ge.startGameLoop();
			cout << "Winner: " << winner << endl;
			winners.push_back(winner);
		}
	}

	//display tournament info
	cout << endl << "The tournament was played in the following way:" << endl;
	cout << "M: ";
	for (int m = 0; m < maps->size(); m++)
		cout << maps->at(m)->getName() << ", ";
	cout << endl;
	cout << "P: ";
	for (int p = 0; p < playerStrategies->size(); p++) {
		switch (playerStrategies->at(p)) {
		case 'a': cout << "Agressive, "; break;
		case 'b': cout << "Benevolent, "; break;
		case 'r': cout << "Random, "; break;
		case 'c': cout << "Cheater, "; break;
		default:break;
		}
	}
	cout << endl;
	cout << "G: " << *gamesPerMap << endl;
	cout << "D: " << *maxTurns << endl;

	//print results
	cout << endl << "TOURNAMENT RESULTS:" << endl << endl;
	cout << left << setw(14) << setfill(' ') << " ";
	for (int g = 0; g < *gamesPerMap; g++) {
		string gameString = "Game " + to_string(g+1);
		cout << left << setw(14) << setfill(' ') << gameString ;
	}
	cout << endl;
	int winnersIndex = 0;
	for (int m = 0; m < maps->size(); m++) {
		cout << left << setw(14) << setfill(' ') << maps->at(m)->getName();
		for(int g = 0; g < *gamesPerMap; g++)
			cout << left << setw(14) << setfill(' ') << winners.at(winnersIndex++);
		cout << endl;
	}

	
}


