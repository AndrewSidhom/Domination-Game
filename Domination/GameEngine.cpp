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
		cout << (players + i)->getName() << endl;
	}	
	assignCountriesToPlayers(gameMap);	
}
// Destructor
GameEngine::~GameEngine() {
	delete [] players;
	players = nullptr;
	delete NUM_OF_COUNTRIES, NUM_OF_PLAYERS;
}

GameEngine::GameEngine(Player *testPlayers, int numOfPlayers, int numTotalCountries) {

	players = new Player[numOfPlayers];
	//cout << "HELLO " << testPlayers;
	players = testPlayers;
	//cout << "HEY " << players;
	NUM_OF_PLAYERS = new int(numOfPlayers);
	NUM_OF_COUNTRIES = new int(numTotalCountries);
}

void GameEngine::startGameLoop() {
    
    int curPlayerIndex = 0; // index of current player's turn
	int turn = 1;	// TODO remove test code after phase 2
    do {
        while(players[curPlayerIndex].getNumOfOwnedCountries() == 0) 
            { curPlayerIndex ++; }  // skip turn if current player has no countries left

		// TODO remove test code below after phase 2 ************************
		cout << "\nTURN " << turn << ", PLAYER " << curPlayerIndex + 1 << "'s Turn!\n";
		cout << "Calling player's reinforce, attack, and fortify funcs\n";
		if(curPlayerIndex == 0 && turn == 4) 
		{
			cout << "\nPlayer 1 should now claim player 2's country and player 2's turn is skipped\n";
			Country* c = players[1].loseCountry(2);	// param: country id
			players[0].claimCountry(c, 1);	// param: country ptr, army size
		}
		else if(curPlayerIndex == 0 && turn == 6)
		{
			cout << "\nPlayer 1 should now claim player 3's country and win the game.\n";
			Country* c = players[2].loseCountry(3);	// param: country id
			players[0].claimCountry(c, 1);	// param: country ptr, army size
		}
		turn++;
		// *******************************************************************

		//Expected code after testing phase 2
        //players[curPlayerIndex].reinforce();
        //players[curPlayerIndex].attack();
        //players[curPlayerIndex].fortify();
		
        if(curPlayerIndex == *NUM_OF_PLAYERS - 1)
            curPlayerIndex = 0;
		else
			curPlayerIndex++;
    } 
    while(!aPlayerOwnsAllCountries());

	// TODO REMOVE CODE after phase 2 testing
	if(players[2].getNumOfOwnedCountries() == 0 &&
		players[1].getNumOfOwnedCountries() == 0 &&
		players[0].getNumOfOwnedCountries() == 3)
			{ cout << "\nPlayer 1 has won the game!"; }
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

void GameEngine::setupPlayers(string name, Deck *deck, Map *gameMap) {
	int numOfPlayers;
	numOfPlayers = queryNumOfPlayers(name);
	NUM_OF_PLAYERS = new int(numOfPlayers);
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

	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		cout << "\n" << players[i].getName() << "'s countries:" << endl;
		for (Country* x : ownedCountries[i]) {
			cout << x->name << endl;
		}
	}
	
	for (int i = 0; i < *NUM_OF_PLAYERS; i++) {
		players[i].setOwnedCountries(ownedCountries[i]);
	}
	//WORK IN PROGRESS...
}