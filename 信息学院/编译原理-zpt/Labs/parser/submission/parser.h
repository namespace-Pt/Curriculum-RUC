#pragma once
#include "grammer.h"

#define GRAMNUM 20
#define SYMNUM 10

class Parser{
    public:
        Parser(Grammers&,Symbols&,Symbols&,Symbols&);
        void getSymbols();
        void initialize(const string&);
        void getFirst();
        void getFollow();
        void getTable();
        void parse(string,string);
        void error();

        Grammer *** table;
    private:
        Grammers grammers;
        Symbols symbols;
        Symbols terminals;
        Symbols nterminals;

        int getLex(vector<deque<Symbol*>>&, string&, string&);
};