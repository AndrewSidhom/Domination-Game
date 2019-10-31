#include "GameEngine.h"
#include "MapLoader.h"
#include <string>
using std::cout;
using std::cin;
using std::endl;

int* Player::currentGenId = new int(1);

// Constructor
GameEngine::GameEngine() {
	int numOfPlayers, numOfCountries;
	string name, mapName;
	cout << "Welcome to Risk, player!" << endl;
	cout << "Enter a name for yourself: ";
	cin >> name;	

	//load map and create deck according to number of countries 
	cout << "Hello " << name << "! Enter the name of the map you would like to load (without the .map extension): ";
	cin >> mapName;
	Map *gameMap = MapLoader::loadMapFile(mapName);
	while (gameMap == nullptr) {
		cout << "Enter the name of the map you would like to load (without the .map extension): ";
		cin >> mapName;
		gameMap = MapLoader::loadMapFile(mapName);
	}
	list<Country> countries = gameMap->getCountries();
	numOfCountries = (int)countries.size();
	NUM_OF_COUNTRIES = new int(numOfCountries);
	Deck deck(*NUM_OF_COUNTRIES);

	//get number of players, set their names and assign them the deck
	numOfPlayers = queryNumOfPlayers(name);
	NUM_OF_PLAYERS = new int(numOfPlayers);
	players = new Player[*NUM_OF_PLAYERS];
	Player *player = new Player(name, &deck);
	*players = *player; //Assign player one first position of players array
	for (int i = 1; i < *NUM_OF_PLAYERS; i++) {
		string input;
		cout << (players + i)->getName() << ", enter your new name, or enter '0' to keep your current name: ";
		cin >> name;
		if (name != "0") {
			(players + i)->setName(name);
		}
		(players + i)->setHand(&deck);
	}

	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {	
		cout << (players + i)->getName() << endl;
	}	
}
// Destructor
GameEngine::~GameEngine() {
	delete [] players;
	players = nullptr;
	delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
}

void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
    do {
        while(players[curPlayerIndex].getNumOfOwnedCountries == 0) 
            { curPlayerIndex ++; }  // skip turn if current player has no countries left

        //players[curPlayerIndex].reinforce();
        //players[curPlayerIndex].attack();
        //players[curPlayerIndex].fortify();

        if(curPlayerIndex == *NUM_OF_PLAYERS - 1)
            { curPlayerIndex = 0; }
    } 
    while(!aPlayerOwnsAllCountries());
}

bool GameEngine::aPlayerOwnsAllCountries() {

    for(int i = 0; i < *NUM_OF_PLAYERS; i++) {
        if(players[i].getNumOfOwnedCountries() == *NUM_OF_COUNTRIES) 
            return true;
    }

    return false;
}

int GameEngine::queryNumOfPlayers(string name) {
	string input;
	bool isValid = false;
	do {
		cout << "How many players (including yourself) will be playing? (2-6 players): ";
		cin >> input;
		if(input == "2" || input == "3" || input == "4" || input == "5" || input == "6")
			isValid = true;
		else
			cout << "Error: Invalid input" << endl;
				
	} while (!isValid);
	int numOfPlayers = stoi(input);
	return numOfPlayers;
}