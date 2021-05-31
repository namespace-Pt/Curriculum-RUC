#include "parser.h"
#include "lexer.cpp"

// print contents of a stack
void printStack(deque<Symbol*> &s){
    for(auto i = s.cbegin();i != s.cend();i++){
        cout<<(*i)->symbol;
    }
}

// grammers, symbols, terminals, nterminals
Parser::Parser(Grammers &g, Symbols &sym, Symbols &term, Symbols &nterm){
    grammers = g;
    symbols = sym;
    terminals = term;
    nterminals = nterm;

    table = nullptr;
}

// read grammers and construct symbols
void Parser::getSymbols(){
    for(int i = 0; i < grammers.len(); i++){
        nterminals.append(grammers[i].head);

        for(int j = 0;j < grammers[i].tail.length();j++){
            if(!symbols.find(grammers[i].tail[j])){
                // append terminals
                Symbol * sym = new Symbol(grammers[i].tail[j], terminals.symbols.size(), true);
                terminals.append(sym);
                symbols.append(sym);
            }
        }
    }

    // append #
    Symbol * sym = new Symbol('#', terminals.symbols.size(), true);
    terminals.append(sym);
    symbols.append(sym);

    char isN = '0';
    for(auto i = symbols.symbols.cbegin(); i != symbols.symbols.cend(); i++){
        isN = '0';
        for(auto j = nterminals.symbols.cbegin(); j != nterminals.symbols.cend(); j++){
            if(i->first == j->first){
                isN = '1';
                break;
            }
        }
        if(isN == '0'){
            terminals.append(i->second);
        }
    }
}

// get the FIRST SET of each symbol
void Parser::getFirst(){
    // add all terminals to their FIRST SET
    for(auto i=terminals.symbols.cbegin();i!=terminals.symbols.cend();i++){
        i->second->append_first(i->second,0);
    }

    for(int i = 0;i < grammers.len();i++){
        // for grammer like A->a, add a into A's FIRST SET
        Symbol* s = terminals.find(grammers[i].tail[0]);
        if(s){
            grammers[i].head->append_first(s,0);
        }
    }

    // for grammer like A->B..., add all of symbols in B's FIRST SET except e into A's FIRST SET
    bool next = false, isChange = true;
    int j = 0;
    // 8 iterations
    while(isChange){
        // if there's no change, break
        isChange=false;

        for(int i = 0;i < grammers.len();i++){
            next = true;
            j = 0;

            Symbol * head = grammers[i].head;
            string tail = grammers[i].tail;

            while(next){
                Symbol * s = nterminals.find(tail[j]);
                if(s){
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
            if(j == tail.length()){
                if(!head->append_first(terminals.find('e'),0)){
                    isChange=true;
                };
            }
        }
    }
}

// get the FOLLOW SET for each symbol
void Parser::getFollow(){
    // add # into the FOLLOW SET of the start symbol
    grammers[0].head->append_follow(terminals.find('#'),0);

    // for grammer like A->..BC.., add all of symbols in C's FIRST SET except epsilon into B's FOLLOW SET
    for(int i = 0;i < grammers.len();i++){
        string tail = grammers[i].tail;
        for(int j = 0;j < tail.length() - 1;j++){
            if(nterminals.find(tail[j])){
                nterminals.find(tail[j])->append_follow(symbols.find(tail[j+1]),1);
            }
        }
    }

    // for grammer like A->..B, add all of symbols in A's FOLLOW SET except epsilon into B's FOLLOW SET
    bool isChange = true;
    // 2 iterations
    while(isChange){
        // if there's no change, break
        isChange=false;

        for(int i = 0;i < grammers.len();i++){
            Symbol * head = grammers[i].head;
            string tail = grammers[i].tail;

            // A->..B or A->..BC, C->e
            Symbol * end = nterminals.find(tail[tail.length() - 1]);
            // skip the one that A->...A
            if(end == head){
                continue;
            }

            if(end){
                if(!end->append_follow(head,2)){
                    isChange=true;
                }

                if(nterminals.find(tail[tail.length() - 2]) && end->produce_e){
                    if(!nterminals.find(tail[tail.length() - 2])->append_follow(head,2)){
                        isChange = true;
                    }
                }
            }
        }
    }
}

// initialize the analysis table and fill it with grammer pointers according to SELECT SET
void Parser::getTable(){
    table = new Grammer**[nterminals.len()];
    for(int i = 0;i < nterminals.len();i++){
        table[i] = new Grammer*[terminals.len()];
        for(int j = 0;j < terminals.len();j++){
            table[i][j] = nullptr;
        }
    }

    for(int i = 0;i < grammers.len(); i++){
        Symbol * head = grammers[i].head;
        for(auto j = grammers[i].select.cbegin();j != grammers[i].select.cend();j++){
            table[head->index][(*j)->index] = &grammers[i];
        }
    }
}

// read grammers and load symbols, args: path of input grammers
void Parser::initialize(const string &path){
    ifstream fin;
    fin.open(path);
    grammers.getGrammer(fin,symbols);
    fin.close();

    getSymbols();
    grammers.getProduceE(symbols);
    getFirst();
    getFollow();
    grammers.getSelect(terminals, nterminals);

    getTable();

    cout<<"Initialization Accomplished!"<<endl;
    grammers.info();
    terminals.info();
    nterminals.info();

    cout<<"\nAnalysis Table is:"<<endl;
    for(int i = 0;i < nterminals.len();i++){
        for(int j = 0;j < terminals.len();j++){
            if(table[i][j]){
                cout<<table[i][j]->head->symbol<<"->"<<table[i][j]->tail<<',';
            }
            else{
                cout<<"Null,";
            }
        }
        cout<<endl;
    }
}

// lexically analyse each input string in the given file, parse it to sentence pattern
int Parser::getLex(vector<deque<Symbol*>> &sentences, string &INPUT, string &OUTPUT){
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

// semantically analyse the sentence
void Parser::parse(string INPUT="data/test.in", string OUTPUT="data/lex_result.out"){
    vector<deque<Symbol*>> sentences;
    deque<Symbol*> sentence;
    deque<Symbol*> stack;
    if(getLex(sentences, INPUT, OUTPUT)){
        return;
    }

    Symbol * symbol;
    Symbol * signal;
    Symbol * tmp;
    Grammer * gram;
    string format;


    cout<<"\nThe symbol stack and the sentence during parsing are: (left as top)"<<endl;
    for(int i = 0;i < sentences.size();i++){
        cout<<"\nsentence: "<<i<<endl;
        sentence = sentences[i];
        stack.push_front(symbols.find('#'));
        stack.push_front(symbols.find('S'));

        while((!stack.empty()) && (!sentence.empty())){
            printStack(stack);
            int tmp = 10 - stack.size();
            format.clear();
            for(int i = 0;i < tmp;i++){
                format += ' ';
            }
            cout<<format;
            printStack(sentence);

            symbol = stack.front();
            signal = sentence.front();

            if(symbol->is_terminal){
                // pop off epsilon
                if(symbol->symbol == 'e'){
                    stack.pop_front();
                }
                else if(signal == symbol){
                    stack.pop_front();
                    sentence.pop_front();
                }
                else{
                    error();
                    return;
                }
                cout<<endl;
            }
            else{
                gram = table[symbol->index][signal->index];
                if(gram){
                    int tmp = 15 - sentence.size();
                    format.clear();
                    for(int i = 0;i < tmp;i++){
                        format += ' ';
                    }
                    cout<<format<< gram->head->symbol <<"->"<< gram->tail<<endl;
                    stack.pop_front();
                    // put the rhs of grammer into stack in reverse order
                    for(int j = gram->tail.length()-1;j >= 0;j--){
                        stack.push_front(symbols.find(gram->tail[j]));
                    }
                }
                else{
                    error();
                    return;
                }
            }
        }
    }

    cout<<"\nSUCCESS!"<<endl;
    return;
}