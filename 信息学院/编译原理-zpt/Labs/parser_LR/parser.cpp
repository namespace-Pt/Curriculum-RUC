#include "parser.h"
#include "lexer.cpp"
#include "grammer.cpp"
#include "grammers.cpp"
#include "symbol.cpp"
#include "item.cpp"
#include "closure.cpp"

// print contents of a stack
void printStack(deque<Symbol*> &s){
    for(auto i = s.cbegin();i != s.cend();i++){
        cout<<(*i)->symbol;
    }
}

void printStack(deque<int> &s){
    for(auto i : s){
        cout<<i<<',';
    }
}

int Parser::_id(char c){
    return (int)c-4;
}

// grammers, symbols, terminals, nterminals
Parser::Parser(){
    action = nullptr;
    goTo = nullptr;
}

// read grammers and construct symbols
void Parser::get_symbols(){
    for(int i = 0; i < grammers.length; i++){
        for(auto j = grammers[i]->tail.cbegin();j != grammers[i]->tail.cend();j++){
            if(!nterminals.find(*j)){
                (*j)->is_terminal = true;
                terminals.append(*j);
            }
        }
    }

    // append #
    Symbol * sym = new Symbol('#', terminals.symbols.size(), true);
    symbols['#'] = sym;
    terminals.append(sym);

    terminals.length = terminals.symbols.size();
    nterminals.length = nterminals.symbols.size();
}

// get the FIRST SET of each symbol
void Parser::getFirst(){
    // add all terminals to their FIRST SET
    for(auto i=terminals.symbols.cbegin();i!=terminals.symbols.cend();i++){
        (*i)->append_first(*i,0);
    }

    for(int i = 0;i < grammers.length;i++){
        // for grammer like A->a, add a into A's FIRST SET
        Symbol* s = terminals.find(grammers[i]->tail[0]);
        if(s){
            grammers[i]->head->append_first(s,0);
        }
    }

    // for grammer like A->B..., add all of symbols in B's FIRST SET except e into A's FIRST SET
    bool next = false, isChange = true;
    int j = 0;
    // 8 iterations
    while(isChange){
        // if there's no change, break
        isChange=false;

        for(int i = 0;i < grammers.length;i++){
            next = true;
            j = 0;

            Symbol * head = grammers[i]->head;
            vector<Symbol*>& tail = grammers[i]->tail;

            while(next){
                Symbol * s = tail[j];
                if(!s->is_terminal){
                    if(!head->append_first(s,1)){
                        isChange=true;
                    };

                    // if the current nonterminal can derive epsilon, then the FIRST SET of its next symbol should also be added
                    if(s->produce_e){
                        next = true;
                        j++;
                    }
                    else{
                        next = false;
                    }
                }
                else{
                    next = false;
                }
            }
            // if all rhs symbols derive epsilon, then epsilon belongs to the FIRST SET
            if(j == tail.size()){
                if(!head->append_first(symbols['e'],0)){
                    isChange=true;
                };
            }
        }
    }
}

// get the FOLLOW SET for each symbol
void Parser::getFollow(){
    // add # into the FOLLOW SET of the start symbol
    grammers[0]->head->append_follow(symbols['#'],0);

    // for grammer like A->..BC.., add all of symbols in C's FIRST SET except epsilon into B's FOLLOW SET
    for(int i = 0;i < grammers.length;i++){
        vector<Symbol*>& tail = grammers[i]->tail;
        for(int j = 0; j < tail.size() - 1; j++){
            if(!tail[j]->is_terminal){
                tail[j]->append_follow(tail[j+1],1);
            }
        }
    }

    // for grammer like A->..B, add all of symbols in A's FOLLOW SET except epsilon into B's FOLLOW SET
    bool isChange = true;
    // 2 iterations
    while(isChange){
        // if there's no change, break
        isChange=false;

        for(int i = 0;i < grammers.length;i++){
            Symbol * head = grammers[i]->head;
            vector<Symbol*>& tail = grammers[i]->tail;

            // A->..B or A->..BC, C->e
            Symbol * end = tail[tail.size() - 1];
            // skip the one that A->...A
            if(end == head){
                continue;
            }

            if(!end->is_terminal){
                if(!end->append_follow(head,2)){
                    isChange=true;
                }

                if(tail.size() > 1 && !tail[tail.size() - 2]->is_terminal && end->produce_e){
                    if(!tail[tail.size() - 2]->append_follow(head,2)){
                        isChange = true;
                    }
                }
            }
        }
    }
}

// initialize the analysis table and fill it with grammer pointers according to SELECT SET
void Parser::get_table(){
    // two additional signal for acceptance and forward
    Symbol * s = new Symbol('@',0,false);
    Grammer * acc = new Grammer(s);

    action = new Grammer**[closures.length];
    for(int i = 0;i < closures.length;i++){
        action[i] = new Grammer*[terminals.length];
        for(int j = 0;j < terminals.length;j++){
            action[i][j] = nullptr;
        }
    }

    goTo = new int*[closures.length];
    for(int i = 0;i < closures.length;i++){
        goTo[i] = new int[nterminals.length];
        for(int j = 0;j < nterminals.length;j++){
            goTo[i][j] = -1;
        }
    }

    Symbol * tmp = nullptr, * head = nullptr, * sym = nullptr, *r = nullptr;
    Grammer * move = nullptr;
    for(auto clos : closures.closures){
        for(auto item : clos->items){
            // move
            if(item->next){
                tmp = item->get_dot();

                Items* next = clos->find_route(tmp->symbol);

                if(tmp->is_terminal){
                    // use  for the 0th closure, such grammer has no tail
                    r = new Symbol(next->index+4,0,false);
                    move = new Grammer(r);
                    action[clos->index][tmp->index] = move;
                }
                else{
                    goTo[clos->index][tmp->index] = next->index;
                }
            }

            // reduce
            else{
                tmp = item->get_head();
                if(tmp->symbol == STARTSYM){
                    action[clos->index][symbols['#']->index] = acc;
                }
                else{
                    for(auto s : tmp->follow){
                        action[clos->index][s->index] = item->grammer;
                    }
                }
            }
        }
    }

}

// read grammers and load symbols, args: path of input grammers
void Parser::initialize(const string &path){
    ifstream fin;
    fin.open(path);
    grammers.get_grammer(fin,symbols,nterminals);
    fin.close();

    get_symbols();
    // grammers.get_produce_e(nterminals);
    getFirst();
    getFollow();

    closures.get_closure_and_go(grammers.items, symbols);
    get_table();

    cout<<"Initialization Accomplished!"<<endl;
    grammers.info();
    terminals.info();
    nterminals.info();
    closures.info();

    cout<<"\nACTION Table is:"<<endl;
    Grammer * tmp = nullptr;
    for(int i = 0;i < closures.length;i++){
        for(int j = 0;j < terminals.length;j++){
            tmp = action[i][j];
            if(tmp){
                if(!tmp->tail.empty()){
                    cout<<tmp->head->symbol<<"->";
                    for(auto it=tmp->tail.cbegin();it != tmp->tail.cend();it++){
                        cout<<(*it)->symbol;
                    }
                    cout<<',';
                }
                else if(tmp->head->symbol == '@'){
                    cout<<"Accept,";
                }
                else{
                    cout<<"move in S"<<_id(tmp->head->symbol)<<',';
                }
            }
            else{
                cout<<"Null,";
            }
        }
        cout<<endl;
    }
    cout<<"\nGOTO Table is:"<<endl;
    for(int i = 0;i < closures.length;i++){
        for(int j = 0;j < nterminals.length;j++){
            cout << goTo[i][j]<<',';
        }
        cout<<endl;
    }
}

// lexically analyse each input string in the given file, parse it to sentence pattern
int Parser::get_lex(vector<deque<Symbol*>> &sentences, string &INPUT, string &OUTPUT){
    lex(INPUT,OUTPUT);

    ifstream fin(OUTPUT);
    string val, type, index, line;

    int i = 0;
    deque<Symbol*> tmp;

    while(getline(fin,line)){
        ++i;

        stringstream ss(line);
        getline(ss, val, '\t');
        getline(ss, type, '\t');

        if(type == "CONST" || type == "IDENTIFIER"){
            tmp.push_back(terminals.find('i'));
        }
        else if(type == "OPERATOR" || type == "SEPARATOR"){
            tmp.push_back(terminals.find(val[0]));
        }
        else if(type == "NEWLINE"){
            tmp.push_back(terminals.find('#'));
            sentences.push_back(tmp);
            tmp.clear();
        }
        else{
            // error
            cout<<"\nNot supported lex "<<type<<":"<<val<<" at line "<<i<<endl;
            return -1;
        }
    }
    // add the last line
    tmp.push_back(terminals.find('#'));
    sentences.push_back(tmp);
    tmp.clear();

    cout<<"\nThe expression for parser is:\n";
    for(auto it = sentences.begin();it != sentences.end();it++){
        printStack(*it);
        cout<<endl;
    }
    return 0;
}

void Parser::error(){
    cout<<"error";
}

void Parser::parse(string INPUT="data/test.in", string OUTPUT="data/lex_result.out"){
    vector<deque<Symbol*>> sentences;
    deque<Symbol*> sentence;
    deque<int> states;
    deque<Symbol*> signals;
    if(get_lex(sentences, INPUT, OUTPUT)){
        return;
    }

    int state,tmp = 0;
    int next = -1;
    Symbol * symbol = nullptr, *signal = nullptr;
    Grammer * act = nullptr;
    string format;

    cout<<"\nThe state stack, symbol stack and the sentence during parsing are: (left as the top of stack)"<<endl;
    for(int i = 0;i < sentences.size();i++){
        cout<<"\nsentence: "<<i<<endl;
        sentence = sentences[i];
        states.push_front(0);
        signals.push_front(symbols['#']);

        while(!sentence.empty()){
            printStack(states);
            tmp = 20 - 2*states.size();
            format.clear();
            for(int i = 0;i < tmp;i++){
                format += ' ';
            }
            cout<<format;
            printStack(signals);
            tmp = 20 - signals.size();
            format.clear();
            for(int i = 0;i < tmp;i++){
                format += ' ';
            }
            cout<<format;
            printStack(sentence);

            state = states.front();
            symbol = sentence.front();

            act = action[state][symbol->index];
            if(act){
                // if move in or accept
                if(act->tail.empty()){
                    if(act->head->symbol == '@'){
                        cout<<"\nSUCCESS!"<<endl;
                        states.clear();
                        signals.clear();
                        sentence.clear();
                    }
                    else{
                        states.push_front(_id(act->head->symbol));
                        signals.push_front(sentence.front());
                        sentence.pop_front();
                    }
                }

                // reduction
                else{
                    // pop off signals and states at the same time, which means one signal corresponding to one state
                    for(auto s=act->tail.crbegin();s!=act->tail.crend();s++){
                        signal = signals.front();
                        signals.pop_front();
                        states.pop_front();
                        // if(signal != *s){
                        //     error();
                        //     return;
                        // }
                    }
                    signals.push_front(act->head);
                    state = states.front();
                    next = goTo[state][act->head->index];
                    if(next == -1){
                        error();
                        return;
                    }
                    states.push_front(next);
                }
            }

            else{
                error();
                return;
            }

            cout<<endl;
        }
    }
}