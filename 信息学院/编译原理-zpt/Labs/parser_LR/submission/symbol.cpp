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

Symbols::Symbols(){
}

// insert an existing symbol into symbols, update its index
void Symbols::append(Symbol* sym){
    if(symbols.find(sym) == symbols.end()){
        sym->index = symbols.size();
        symbols.insert(sym);
    }
    return;
}

// find a symbol from symbols according to char, return its iterator
Symbol* Symbols::find(char c){
    for(auto it = symbols.cbegin();it != symbols.cend();it++){
        if((*it)->symbol == c){
            return *it;
        }
    }
    return nullptr;
}

// find a symbol from symbols, return its iterator, if not found, return nullptr
Symbol* Symbols::find(Symbol *s){
    auto a = symbols.find(s);
    if(a == symbols.end()){
        return nullptr;
    }
    else{
        return *a;
    }
}

// print all symbols
void Symbols::info(){
    cout<<"\nThere are "<<symbols.size()<<" symbols, containing: \n";
    for(auto i = symbols.cbegin();i != symbols.cend(); i++){
        cout<<(*i)->index<<':'<<(*i)->symbol<<" t:"<<(*i)->is_terminal<<" e:"<<(*i)->produce_e<<" its FIRST set is: ";
        (*i)->printFirst();
        cout<<" its FOLLOW set is: ";
        (*i)->printFollow();
        cout<<endl;
    }
}