#include"grammer.h"

Grammer::Grammer(){
    head = nullptr;
}

void Grammer::printSelect(){
    for(auto i=select.cbegin();i!=select.cend();i++){
        cout<<(*i)->symbol;
    }
}

Grammers::Grammers(){
    length = 0;
    grammers = nullptr;
}

Grammers::Grammers(int gramNum){
    length = 0;
    grammers = new Grammer[gramNum];
}

Grammer& Grammers::operator[](int i){
    return grammers[i];
}

// returns: total number of the grammers
int Grammers::len(){
    return length;
}

// read the grammer file into grammers
void Grammers::getGrammer(ifstream &fin, Symbols &syms){
    string line;
    int i = 0;
    while(getline(fin,line)){
        Symbol* head = syms.find(line[0]);
        if(!head){
            grammers[i].head = new Symbol(line[0],syms.symbols.size(),false);
            syms.append(grammers[i].head);
        }
        else{
            grammers[i].head = head;
        }
        grammers[i].tail = line.substr(3,line.length()-3);
        i++;
    }
    length = i;
}

// check if a given sym can produce epsilon recursively
bool Grammers::_produceE(char sym){
    if(sym == 'e'){
        return true;
    }

    string target;
    bool a[GRAMNUM] = {false};

    for(int i = 0;i < length;i++){
        if(grammers[i].head->symbol == sym){
            target = grammers[i].tail;
            a[i] = true;
            for(int j = 0;j < target.length();j++){
                a[i] = a[i] && _produceE(target[j]);
                // early stopping if there is a symbol that cannot derive epsilon
                if(!a[i]){
                    break;
                }
            }
        }
    }

    for(int i = 0;i < GRAMNUM;i++){
        if(a[i] == true){
            return true;
        }
    }
    return false;
}

// find out symbols that can produce epsilon
void Grammers::getProduceE(Symbols &syms){
    for(auto i=syms.symbols.cbegin();i !=syms.symbols.cend();i++){
        i->second->produce_e = _produceE(i->first);
    }
}

// find out SELECT SET of each grammer
void Grammers::getSelect(Symbols &term, Symbols &nterm){
    for(int i = 0;i < this->length;i++){
        if(grammers[i].tail[0] == 'e'){
            grammers[i].select = grammers[i].head->follow;
        }
        else if(term.find(grammers[i].tail[0])){
            grammers[i].select = term.find(grammers[i].tail[0])->first;
        }
        else if(nterm.find(grammers[i].tail[0])){
            grammers[i].select = nterm.find(grammers[i].tail[0])->first;
            if(nterm.find(grammers[i].tail[0])->produce_e){
                for(auto it=grammers[i].head->follow.begin();it != grammers[i].head->follow.end();it++){
                    grammers[i].select.insert(*it);
                }
            }
        }
    }
}

// print all grammers
void Grammers::info(){
    cout<<"There are "<<length<<" grammers, containing:"<<endl;
    for(int i = 0;i < length;i++){
        cout<<grammers[i].head->symbol<<"->"<<grammers[i].tail<<" its SELECT SET is ";
        grammers[i].printSelect();
        cout<<endl;
    }
}