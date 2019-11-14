#pragma once
#include <string>
#include "GameObservers.h"

class PhaseLog : Subject {

public:
    PhaseLog();
    ~PhaseLog();
    PhaseLog(const PhaseLog &pl);
    PhaseLog& operator = (const PhaseLog *pl);

    // Pass string msg and notify observers to print msg
    void printMsg(string s);
    // Accessor
    string* getMsg() { return msg; };

private:
    string* msg;
};