# include<iostream>
# include<fstream>
# include<ctype.h>
# include<string.h>
# include<map>
# include<string>
# include<vector>

using namespace std;

char ch;
char next;
string token;

map<char,int> SEPARATORS;
map<string,int> OPERATORS;
map<string,int> KEYS;
map<string,int> IDENTIFIERS;
map<string,int> CONSTS;

void printMap(map<string,int> dict, ofstream * fout = nullptr){
    for(auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
        cout << it->first << " " << it->second<< "\n";
        if(fout){
            *(fout) << it->first << " " << it->second<< "\n";
        }
    }
}

void printMap(map<char,int> dict, ofstream * fout = nullptr){
    for(auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
        cout << it->first << " " << it->second<< "\n";
        if(fout){
            *(fout) << it->first << " " << it->second<< "\n";
        }
    }
}

istream& getChar(ifstream &fin, char &c){
    return fin.get(c);
}

char peekChar(ifstream &fin, char &c){
    c = fin.peek();
    return fin.peek();
}

int isLetter(char &c){
    return isalpha(c);
}

int isDigit(char &c){
    return isdigit(c);
}

int isOperator(char &c){
    string s;
    s += c;
    if(OPERATORS.find(s) != OPERATORS.end()){
        return 1;
    }
    else{
        return 0;
    }
}

int read_predefined(const char * keyPath, const char * sepPath, const char * opPath){
    // load reserved keywords, separators and operators
    string tmps;
    char tmpc;
    int i = 0;
    ifstream keyFile(keyPath);
    ifstream sepFile(sepPath);
    ifstream opFile(opPath);

    while(keyFile >> tmps){
        KEYS[tmps] = i;
        ++i;
    }

    i = 0;
    while(sepFile >> tmpc){
        SEPARATORS[tmpc] = i;
        ++i;
    }

    i = 0;
    while(opFile >> tmps){
        OPERATORS[tmps] = i;
        ++i;
    }
    return 0;
}

void initialize(){
    // load predefined maps

    // FIXME, raise error if the file does not exist
    const char * keyPath = "data/KeyWords.txt";
    const char * sepPath = "data/Separators.txt";
    const char * opPath = "data/Operators.txt";
    const char * typePath = "data/Types.txt";

    read_predefined(keyPath, sepPath, opPath);
    cout<<"initialization accomplished";
    printMap(KEYS);
    printMap(SEPARATORS);
    printMap(OPERATORS);

}

void output(ofstream &fout, const string &attr, string &value, int index){
    fout << value << '\t' << attr << '\t' << index << endl;
}

int isIn(char &c, const string &str){
    for(int i = 0;i < str.size();i++){
        if(c == str[i]) return 1;
    }
    return 0;
}

void parse_error(ifstream &fin, ofstream &fout, bool append=true){
    /* omit current error token and output error */
    // append the current ch
    if (append) token += ch;
    while(getChar(fin,ch) && !isIn(ch,"\n ")){
        token += ch;
    }
    output(fout, "ERROR", token, 0);
    token.clear();
}

void parse_comment_line(ifstream&fin, ofstream &fout){
    /* omit current line and output comment */
    while(getChar(fin,ch) && !isIn(ch,"\n")){
        token += ch;
    }
    output(fout, "COMMENT", token, 0);
    token.clear();
}

void parse_comment_block(ifstream&fin, ofstream &fout){
    /* omit line between comment signals, strip off \n */
    char tmp = '\0';
    while(getChar(fin,ch)){
        if (ch != '\n'){
            token += ch;
        }
        if (ch == '*' && peekChar(fin, tmp) && tmp == '/'){
            getChar(fin, ch);
            token += ch;
            output(fout, "COMMENT", token, 1);
            token.clear();
            return;
        }
    }
    parse_error(fin, fout, false);
}

void parse_keyword_and_identifier(ifstream &fin, ofstream &fout){
    string s;
    token += ch;
    while(getChar(fin,ch) && (isLetter(ch) || isDigit(ch))){
        token += ch;
    }
    if(KEYS.find(token) == KEYS.end()){
        IDENTIFIERS[token] = IDENTIFIERS.size();
        output(fout, "IDENTIFIER", token, IDENTIFIERS[token]);
    }
    else{
        output(fout, "KEYWORD", token, KEYS[token]);
    }
    token.clear();
    // if the following is in separators, operators and spaces, continue
    fin.putback(ch);
    return;
}

void parse_digit_and_decimal(ifstream &fin, ofstream &fout){
    bool isInt = false;
    bool isDecimal = false;
    // scientific notation (e|E)
    bool isSN = false;
    bool isError = false;
    char tmp = '\0';


    // important to parse the digit from zero position
    fin.putback(ch);
    while(getChar(fin, ch) && (isDigit(ch) || isIn(ch, ".eE+-"))){
        switch (ch)
        {
            case '.':
                // 1..2 1.2.3
                if(isDecimal) {
                    parse_error(fin,fout);
                    return;
                }
                token += ch;
                isDecimal = true;
                break;
            case 'e':
            case 'E':
                // 3E4E5
                if(isSN) {
                    parse_error(fin,fout);
                    return;
                }
                token += ch;
                isSN = true;
                isDecimal = false;
                break;
            case '+':
            case '-':
                if (peekChar(fin,tmp) && !isDigit(tmp)){
                    parse_error(fin, fout);
                    return;
                }
                else if (!isSN){
                    CONSTS[token] = CONSTS.size();
                    output(fout, "CONST", token, CONSTS[token]);
                    token.clear();
                    fin.putback(ch);
                    return;
                }
                else{
                    token += ch;
                }
                break;
            default:
                // 0b
                token += ch;
                if(peekChar(fin,ch) && isLetter(ch) && !isIn(ch,"Ee")){
                    parse_error(fin,fout,false);
                    return;
                }
                break;
        }
    }
    // putback the last illegal character because it may be part of others
    fin.putback(ch);

    CONSTS[token] = CONSTS.size();
    output(fout, "CONST", token, CONSTS[token]);

    token.clear();
}

void parse_operator(ifstream &fin, ofstream &fout){
    token += ch;
    switch (ch)
    {
        case '=':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '<':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '>':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '+':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else if (ch == '+'){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '-':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else if (ch == '-'){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '*':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else if (ch == '*'){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '/':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '='){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else if (ch == '/'){
                token += ch;
                parse_comment_line(fin, fout);
                return;
            }
            else if (ch == '*'){
                token += ch;
                parse_comment_block(fin, fout);
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '%':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                token.clear();
                return;
            }

        case '|':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATORr", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '|'){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            else{
                parse_error(fin,fout);
                return;
            }

        case '&':
            getChar(fin,ch);
            if (!isOperator(ch)){
                output(fout, "OPERATOR", token, OPERATORS[token]);
                fin.putback(ch);
                token.clear();
                return;
            }
            else if (ch == '&'){
                token += ch;
                output(fout, "OPERATOR", token, OPERATORS[token]);
                token.clear();
                return;
            }
            // FIXME, any illegal signal next to < should be taken into consideration
            else{
                parse_error(fin,fout);
                return;
            }

        default:
            break;
    }
}

void parse_separator(ifstream &fin, ofstream &fout){
    token += ch;
    switch (ch)
    {
        case '{':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

        case '}':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

        case ';':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

        case ',':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

        case '(':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

        case ')':
            output(fout, "SEPARATOR", token, SEPARATORS[ch]);
            token.clear();
            break;

    default:
        break;
    }
}

void lex(string INPUT="data/test.in", string OUTPUT="result.out"){
    int code = 0;
    char tmp;
    string tmps;
    ifstream fin;
    ofstream fout;
    fin.open(INPUT,ifstream::in);
    fout.open(OUTPUT,ofstream::out);

    const char * keyPath = "data/KeyWords.txt";
    const char * sepPath = "data/Separators.txt";
    const char * opPath = "data/Operators.txt";
    const char * typePath = "data/Types.txt";

    read_predefined(keyPath, sepPath, opPath);

    while(getChar(fin, ch)){
        // make sure ch is not ' '
        if(isIn(ch, "\n\t ")){
            if(ch == '\n'){
                token = "\\n";
                output(fout,"NEWLINE",token,0);
            }
            token.clear();
            continue;
        }

        // Keywords and Identifiers
        if(isLetter(ch)){
            parse_keyword_and_identifier(fin,fout);
        }

        // Digits
        else if(isDigit(ch)){
            parse_digit_and_decimal(fin,fout);
        }

        // Operators
        else if(OPERATORS.find(tmps+ch) != OPERATORS.end()){
            parse_operator(fin,fout);
        }

        // Separators
        else if(SEPARATORS.find(ch) != SEPARATORS.end()){
            parse_separator(fin,fout);
        }

        // Unknown
        else{
            parse_error(fin, fout, true);
        }
    }
    fin.close();
    fout.close();
}