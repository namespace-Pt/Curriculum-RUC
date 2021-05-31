#include "parser.cpp"
#include "grammer.cpp"
#include "symbol.cpp"

int main(){
    Grammers grammers(GRAMNUM);
    Symbols symbols, terminals, nterminals;

    Parser parser(grammers,symbols,terminals,nterminals);
    parser.initialize("data/grammers.txt");
    parser.parse("data/test.in");
    return 0;
}