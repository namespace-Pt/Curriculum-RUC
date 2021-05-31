#include"grammer.h"

Grammer::Grammer(){
    head = nullptr;
}

Grammer::Grammer(Symbol * s){
    head = s;
}

void Grammer::info(){
    cout<<head->symbol<<"->";
    for(auto i : tail){
        cout<<i->symbol;
    }
}