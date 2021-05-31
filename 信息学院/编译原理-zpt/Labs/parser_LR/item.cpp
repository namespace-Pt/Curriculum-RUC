# include "item.h"

Item::Item(){
    grammer = nullptr;
    next = nullptr;
    index = 0;
    dot = 0;
    length = 0;
}

Item::Item(Grammer* g, int d, int i){
    grammer = g;
    next = nullptr;
    dot = d;
    index = i;
    length = g->tail.size();
}

Symbol* Item::get_dot(){
    if(!next){
        return nullptr;
    }
    return grammer->tail[dot];
}

Symbol* Item::get_head(){
    return grammer->head;
}

void Item::info(){
    cout<<grammer->head->symbol << "->";

    for(int k = 0;k < dot;k++){
        cout<<grammer->tail[k]->symbol;
    }
    cout<<'.';
    for(int k = dot;k < length;k++){
        cout<<grammer->tail[k]->symbol;
    }
}

Items::Items(){
    index = 0;
}

Items::Items(int id){
    index = id;
}

// return 0 if successfully appended, 1 otherwise; if toSet=True, then append item index to the set
bool Items::append(Item * item, bool toSet){
    for(auto i : items){
        if(i == item){
            return 1;
        }
    }
    if(toSet){
        indices.insert(item->index);
    }
    items.push_back(item);
    return 0;
}

// FIXME, neatly print
void Items::info(){
    for(auto x : items){
        x->info();
        cout<<"  ";
    }
}

Items* Items::find_route(char c){
    auto r = route.find(c);
    if(r == route.end()){
        return nullptr;
    }
    return r->second;
}