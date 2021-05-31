#pragma once
#include "grammers.h"

class Parser{
    public:
        Parser();
        void get_symbols();
        void initialize(const string&);
        void getFirst();
        void getFollow();
        void get_table();
        void parse(string,string);
        void error();

        Grammer *** action;
        int ** goTo;

    private:
        Grammers grammers;
        map<char, Symbol*> symbols;
        Symbols terminals;
        Symbols nterminals;

        Closures closures;

        int get_lex(vector<deque<Symbol*>>&, string&, string&);
        int _id(char);
};