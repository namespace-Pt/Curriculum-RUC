# include "grammers.h"

Grammers::Grammers(){
    length = 0;
    // item_num = 0;
    // grammers = nullptr;
}

Grammer* Grammers::operator[](int i){
    return grammers[i];
}

vector<Items*>& Grammers::operator[](char sym){
    return items[sym];
}

// returns: total number of the grammers
int Grammers::len(){
    return length;
}

// read the grammer file into grammers
void Grammers::get_grammer(ifstream &fin, map<char, Symbol*> &syms, Symbols &ntms){
    string line;
    int k = 0;
    while(getline(fin,line)){
        Grammer* grammer = new Grammer();

        // create all the symbol instance first to make it a part of the grammer
        auto res = syms.find(line[0]);
        if(res == syms.end()){
            // index of the symbol in symbols doesn't matter, it will be modified in parser::getSymbol()
            Symbol* head = new Symbol(line[0],0,false);
            grammer->head = head;
            syms[line[0]] = head;

            ntms.append(head);
        }
        else{
            grammer->head = res->second;
            ntms.append(res->second);
        }

        string tail = line.substr(3,line.length()-3);
        for(int j = 0;j < tail.length();j++){
            auto res = syms.find(tail[j]);
            if(res == syms.end()){
                Symbol * t = new Symbol(tail[j],0,false);
                grammer->tail.push_back(t);
                syms[t->symbol] = t;
            }
            else{
                grammer->tail.push_back(res->second);
            }
        }

        grammers.push_back(grammer);

        // create items
        Items * items_ = new Items();
        Item* prev_item = nullptr;
        // vector<Item*> tmp;
        for(int j = 0;j <= grammer->tail.size(); j++){
            Item* item = new Item(grammer, j, k++);
            if(prev_item){
                prev_item->next = item;
            }
            prev_item = item;
            // item_vec.push_back(item);
            items_->append(item);
        }

        items[grammer->head->symbol].push_back(items_);
        // items.insert(pair<char, vector<Symbol*>> (head->symbol, tmp));
    }

    length = grammers.size();
    // item_num = k;
}

// print all grammers
void Grammers::info(){
    cout<<"There are "<<length<<" grammers, containing:"<<endl;
    for(int i = 0;i < length;i++){
        cout<<grammers[i]->head->symbol<<"->";
        for(auto j = grammers[i]->tail.cbegin();j != grammers[i]->tail.cend();j++){
            cout<<(*j)->symbol;
        }
        cout<<endl;
    }

    for(auto it=items.cbegin();it != items.cend();it++){
        for(auto jt = it->second.cbegin();jt != it->second.cend(); jt++){
            (*jt)->info();
        }
        cout<<endl;
    }
}