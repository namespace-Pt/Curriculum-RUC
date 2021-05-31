# pragma once
# include "item.h"

class Closures{
    public:
        Closures();
        Closures(int);
        vector<Items*> closures;

        Items* find(Items*);
        void get_closure_and_go(map<char, vector<Items*>>&, map<char, Symbol*>&);
        void info();

        int length;
};