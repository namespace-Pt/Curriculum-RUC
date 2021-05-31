#include "parser.cpp"

int main(){
    Parser parser;
    parser.initialize("data/grammers.txt");
    parser.parse("data/test.in");
    return 0;
}