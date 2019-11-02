//#include <iostream>
//#include "Dice.h"
//using namespace std;
//
///*
//	Class responsible for the testing of class Dice.
//
//	For better test results, the last test, which checks for equal distribution in the rolled
//	numbers, allows an input of 5000.
//*/ 
//int main() {
//
//    Dice d1;
//    Dice d2;
//
//    cout << endl << "Testing if can roll from 1 to 3: " << endl;
//	cout << "(Please input values 1, 2 and 3)" << endl;
//    for(int i = 0; i < 3; i++) {
//        d1.roll();
//    }
//    d1.showInfo();
//
//    cout << endl << "Test if container returned correct and sorted values: " << endl;
//    list<int> c = d2.roll();
//    cout << endl << "Values in container are: " << endl;
//    for(int i : c) {
//        cout << i << endl;
//    }
//
//    cout << endl << "Test if values are always within 1 and 6: " << endl;
//    c = d2.roll();
//    cout << endl << "Values in container are: " << endl;
//    for(int i : c) {
//        cout << i << endl;
//    }
//
//	// For better test results, use an input of 5000 here
//    cout << endl << "Test if rolled numbers are distributed equally and show %: " << endl;
//    d1.rollForTests();
//    d1.showInfo();
//
//	system("pause");
//}