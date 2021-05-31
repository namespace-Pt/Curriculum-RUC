#pragma once
#include<iostream>
#include<string>
#include<map>
#include<set>
#include<deque>
#include<vector>
#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;

// class for a single symbol
class Symbol{
    public:
        char symbol;
        int index;
        bool produce_e;
        bool is_terminal;

        Symbol();
        Symbol(char,int,bool);

        set<Symbol*> first;
        set<Symbol*> follow;

        int append_first(Symbol*,int);

        int append_follow(Symbol*, int);

        void printFirst();
        void printFollow();
};

// class for symbols
class Symbols{
    public:
        Symbols();
        int len();
        void info();

        void append(Symbol*);
        Symbol* find(char);

        map<char,Symbol*> symbols;
};