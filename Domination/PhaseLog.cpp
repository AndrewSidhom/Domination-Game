#include "PhaseLog.h"

PhaseLog::PhaseLog() : msg(new string("")) {}
PhaseLog::~PhaseLog() { delete msg; };
PhaseLog::PhaseLog(const PhaseLog &pl) : msg(pl.msg) {}
PhaseLog& PhaseLog::operator=(const PhaseLog &pl) { 
    msg = pl.msg;
    return *this;
}

void PhaseLog::printMsg(const string &s) {
    delete msg;
    msg = new string(s);
    Notify();
}