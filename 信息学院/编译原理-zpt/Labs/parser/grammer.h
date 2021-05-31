# pragma once
#include "symbol.h"

// class for a single grammer
class Grammer{
    public:
        Grammer();
        Symbol* head;
        string tail;

        set<Symbol*> select;

        void printSelect();
};


// class for grammers
class Grammers{
    public:
        Grammers();
        Grammers(int);
        int len();
        void info();

        Grammer& operator[](int);

        void getGrammer(ifstream&, Symbols&);
        void getProduceE(Symbols&);
        void getSelect(Symbols&,Symbols&);

        Grammer * grammers;

    private:
        int length;
        bool _produceE(char);
};
