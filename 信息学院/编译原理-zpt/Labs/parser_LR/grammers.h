#pragma once
# include "closure.h"

// class for grammers
class Grammers{
    public:
        Grammers();
        // Grammers(int);
        int len();
        void info();

        Grammer* operator[](int);
        vector<Items*>& operator[](char);

        void get_grammer(ifstream&, map<char, Symbol*>&, Symbols&);
        // void get_produce_e(Symbols&);

        vector<Grammer*> grammers;
        map<char, vector<Items*>> items;

        int length;

    // private:
        // bool _produceE(char);
};
