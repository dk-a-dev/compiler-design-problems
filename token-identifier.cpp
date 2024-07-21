#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
using namespace std;

unordered_set<string> keywords={"int", "float", "if", "else", "while", "for", "return"};
unordered_set<char> operators = {'+', '-', '*', '/', '=', '<', '>', '!', '&', '|', '^', '%', '~', '?', ':'};

bool isKeyword(const string &str){
    return keywords.find(str)!=keywords.end();
}

bool isOperator(char ch) {
    return operators.find(ch) != operators.end();
}

bool isNumber(const string& str){
    for(char ch:str){
        if(!isdigit(ch)) return false;
    }
    return true;
}

bool isIdentifier(const string& str){
    if (str.empty() || !isalpha(str[0])) return false;
    for (char ch : str) {
        if (!isalnum(ch) && ch != '_') return false;
    }
    return true;
}

void identifyTokens(const string& statement) {
    vector<string> tokens;
    string token;
    
    for (char ch : statement) {
        if (isspace(ch) || isOperator(ch) || ch == ';' || ch == '(' || ch == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (isOperator(ch) || ch == ';' || ch == '(' || ch == ')') {
                tokens.push_back(string(1, ch));
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }

    for (const string& tok : tokens) {
        if (isKeyword(tok)) {
            cout << tok << " is a keyword" << endl;
        } else if (isNumber(tok)) {
            cout << tok << " is a number" << endl;
        } else if (isIdentifier(tok)) {
            cout << tok << " is an identifier" << endl;
        } else if (tok.size() == 1 && isOperator(tok[0])) {
            cout << tok << " is an operator" << endl;
        } else {
            cout << tok << " is unknown" << endl;
        }
    }
}

int main() {
    string statement;
    cout << "Enter a statement: ";
    getline(cin, statement);
    identifyTokens(statement);
    return 0;
}