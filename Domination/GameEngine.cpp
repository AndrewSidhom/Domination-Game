#include "GameEngine.h"
#include "MapLoader.h"
#include "Cards.h"
#include <string>
using std::cout;
using std::cin;
using std::endl;


// Constructor
GameEngine::GameEngine() {
	//PART 1
	string name;
	cout << "Welcome to Risk, player!" << endl;
	cout << "Enter a name for yourself: ";
	cin >> name;	
	cout << "Hello " << name << "! Enter the name of the map you would like to load (without the .map extension): ";
	//load map and create deck according to number of countries 
	Map *gameMap = loadGameMap();
	Deck deck(*NUM_OF_COUNTRIES);
	//get number of players, set their names and assign them the deck
	setupPlayers(name, &deck, gameMap);

	//PART 2
	randomOrder();
	cout << "\nOrder of players\n----------------" << endl;
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {	
		cout << players[i].getName() << endl;
	}	
	assignCountriesToPlayers(gameMap);
	assignArmiesToCountries();	
}
// Destructor
GameEngine::~GameEngine() {
	delete [] players;
	players = nullptr;
	delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
}

/*	Responsible for starting the game loop. Loop ends when a player owns all countries on map.
	Every loop, each player that owns at least 1 country will call reinforce, attack, fortify in that order.
*/
void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
    do {
        while(players[curPlayerIndex].getNumOfOwnedCountries() == 0) 
            { curPlayerIndex ++; }  // skip turn if current player has no countries left

		players[curPlayerIndex].reinforce();
		players[curPlayerIndex].attack();
		players[curPlayerIndex].fortify();
		
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

// Asks input for the number of Players. Returns the number of players.
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

// Asks input for the map to be use in the game. Returns a pointer to a Map object that corresponds to the loaded map.
Map* GameEngine::loadGameMap() {
	string mapName;
	int numOfCountries;	
	cin >> mapName;
	Map *gameMap = MapLoader::loadMapFile(mapName);
	while (gameMap == nullptr) {
		cout << "Enter the name of the map you would like to load (without the .map extension): ";
		cin >> mapName;
		gameMap = MapLoader::loadMapFile(mapName);
	}
	list<Country*> *countries = gameMap->getCountries();
	numOfCountries = (int)countries->size();
	NUM_OF_COUNTRIES = new int(numOfCountries);

	return gameMap;
}

// Creates the Players. Asks input for Player names.
void GameEngine::setupPlayers(string name, Deck *deck, Map *gameMap) {
	int numOfPlayers;
	numOfPlayers = queryNumOfPlayers(name);
	NUM_OF_PLAYERS = new int(numOfPlayers);
	setStartupArmies();
	players = new Player[*NUM_OF_PLAYERS];
	Player *player = new Player(name, deck, gameMap);
	*players = *player; //Assign player one first position of players array
	for (int i = 1; i < *NUM_OF_PLAYERS; i++) {
		string input;
		cout << (players + i)->getName() << ", enter your new name, or enter '0' to keep your current name: ";
		cin >> name;
		if (name != "0") {
			(players + i)->setName(name);
		}
		(players + i)->setHand(deck);
		(players + i)->setMap(gameMap);
	}
}

// Changes randomly the order of the Players stored in players
void GameEngine::randomOrder() {
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
	vector<int> countryIds;
	for (int i = 1; i <= *NUM_OF_COUNTRIES; i++) {
		countryIds.push_back(i);
	}
	const int COLUMNS = 6;
	int index = 0;
	list<Country*> ownedCountries[COLUMNS];
	int rnd;
	random_device rd;
	mt19937 mt(rd());
	
	for (int i = 0; i < *NUM_OF_COUNTRIES; i++) {
		uniform_int_distribution<int> dist(0, countryIds.size() - 1);
		rnd = dist(mt);
		Country* country = gameMap->getCountryById(countryIds.at(rnd));
		ownedCountries[index].push_back(country);
		countryIds.erase(countryIds.begin() + rnd);
		index = (index + 1) % *NUM_OF_PLAYERS;
	}

	//***THIS LOOP CAN BE REMOVED AFTER DEMO***
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		cout << "\n" << players[i].getName() << "'s countries:" << endl;
		for (Country* x : ownedCountries[i]) {
			cout << x->name << endl;
		}
	}

	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		players[i].setOwnedCountries(ownedCountries[i]);
	}
}

// Determines how many armies should be distributed amongst the Players in the startup phase depending on
// the total number of Players.
void GameEngine::setStartupArmies() {
	switch (*NUM_OF_PLAYERS) {
	case 2:
		STARTUP_ARMIES = new int(40);
		break;
	case 3:
		STARTUP_ARMIES = new int(35);
		break;
	case 4:
		STARTUP_ARMIES = new int(30);
		break;
	case 5:
		STARTUP_ARMIES = new int(25);
		break;
	case 6:
		STARTUP_ARMIES = new int(20);
		break;
	}
}

// Assigns armies to each Country.
void GameEngine::assignArmiesToCountries() {
	int remainingArmies = *STARTUP_ARMIES;
	for (int i = 0; i < *STARTUP_ARMIES; i++) {
		for (int j = 0; j < *NUM_OF_PLAYERS; j++) {
			cout << "\n" << players[j].getName() << "'s turn" << endl;
			players[j].distributeArmies(remainingArmies, true);
			
		}
		remainingArmies--;
	}
}