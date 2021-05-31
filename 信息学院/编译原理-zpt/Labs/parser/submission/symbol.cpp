#include "symbol.h"

Symbol::Symbol(){
    symbol = '\0';
    index = 0;
    produce_e = false;
    is_terminal = false;
}

Symbol::Symbol(char c,int i,bool b=false){
    symbol = c;
    index = i;
    is_terminal = b;
    produce_e = false;
}

// append elements into the FIRST SET of the symbol, returns 1 for collision
int Symbol::append_first(Symbol* s, int mode=0){
    int s1 = first.size();
    switch (mode)
    {
        case 0:
            first.insert(s);
            break;
        case 1:
            for(auto i=s->first.begin();i!=s->first.end();i++){
                first.insert(*i);
            }
            break;
        default:
            // error
            break;
    }
    return first.size() == s1;
}

// append elements into the FIRST SET of the symbol, returns 1 for collision
int Symbol::append_follow(Symbol* s, int mode){
    int s1 = follow.size();
    switch (mode)
    {
        case 0:
            follow.insert(s);
            break;
        case 1:
            for(auto i=s->first.begin();i!=s->first.end();i++){
                follow.insert(*i);
            }
            break;
        case 2:
            for(auto i=s->follow.begin();i!=s->follow.end();i++){
                follow.insert(*i);
            }
            break;
        default:
            // error
            break;
    }
    return follow.size() == s1;
}

void Symbol::printFirst(){
    for(auto i=first.cbegin();i!=first.cend();i++){
        cout<<(*i)->symbol;
    }
}

void Symbol::printFollow(){
    for(auto i=follow.cbegin();i!=follow.cend();i++){
        cout<<(*i)->symbol;
    }
}

int Symbols::len(){
    return symbols.size();
}

Symbols::Symbols(){
}

// insert an existing symbol into symbols
void Symbols::append(Symbol* sym){
    if(symbols.find(sym->symbol) == symbols.end()){
        symbols[sym->symbol] = sym;
    }
    return;
}

// find a symbol from symbols, return its iterator
Symbol* Symbols::find(char c){
    auto a = symbols.find(c);
    if(a == symbols.end()){
        return nullptr;
    }
    else{
        return a->second;
    }
}

// print all symbols
void Symbols::info(){
    cout<<"\nThere are "<<symbols.size()<<" symbols, containing: \n";
    for(auto i = symbols.cbegin();i != symbols.cend(); i++){
        cout<<i->second->index<<':'<<i->first<<" t:"<<i->second->is_terminal<<" e:"<<i->second->produce_e<<" its FIRST set is: ";
        i->second->printFirst();
        cout<<" its FOLLOW set is: ";
        i->second->printFollow();
        cout<<endl;
    }
}