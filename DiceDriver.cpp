#include <iostream>
#include "Dice.h"
using namespace std;

/** Class responsible for the testing of class Dice.
 */ 
int main() {

    Dice d1;
    Dice d2;

    cout << endl << "Testing if can roll from 1 to 3: " << endl;
    for(int i = 0; i < 3; i++) {
        d1.roll();
    }
    d1.showInfo();

    cout << endl << "Test if container returned correct and sorted values: " << endl;
    list<int> c = d2.roll();
    cout << endl << "Values in container are: " << endl;
    for(int i : c) {
        cout << i << endl;
    }

    cout << endl << "Test if values are always within 1 and 6: " << endl;
    c = d2.roll();
    cout << endl << "Values in container are: " << endl;
    for(int i : c) {
        cout << i << endl;
    }

    cout << endl << "Test if rolled numbers are distributed equally and show %: " << endl;
    for(int i = 0; i < 20; i++) {
        d1.roll();
    }
    d1.showInfo();
}