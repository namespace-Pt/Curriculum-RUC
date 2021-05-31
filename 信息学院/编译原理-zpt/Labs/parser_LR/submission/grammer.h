# pragma once
#include "symbol.h"

// class for a single grammer
class Grammer{
    public:
        Grammer();
        Grammer(Symbol*);
        Symbol* head;
        vector<Symbol*> tail;

        void info();
};

