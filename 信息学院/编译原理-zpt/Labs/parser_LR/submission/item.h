# pragma once
# include "grammer.h"

class Item{
    public:
        Item();
        Item(Grammer*, int, int);

        Symbol* get_dot();
        Symbol* get_head();

        void info();

        Grammer* grammer;
        Item* next;

        int dot;
        int index;
        int length;
};

// struct cmp {
//     bool operator() (Item* a, Item* b) const {
//         return a->dot < b->dot;
//     }
// };

class Items{
    public:
        Items();
        Items(int);

        Items* find_route(char);

        bool append(Item *, bool=false);
        void info();
        // bool _cmp(Item* a, Item* b){return a->dot < b->dot;};
        // cannot use set because if use customized comparator, the elements cannot be properly inserted
        vector<Item*> items;
        set<int> indices;
        map<char, Items*> route;
        int index;
};