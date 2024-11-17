#include <iostream>
#include <vector>
#include <string>
using namespace std;

void terminateOnInvalidGrammar() {
    cout << "Not operator grammar" << endl;
    exit(0);
}

int main() {
    int numProductions;  
    vector<string> grammar;
    cout << "Enter the number of productions: ";
    cin >> numProductions;

    cout << "Enter the productions (e.g., E=E+T):" << endl;
    for (int i = 0; i < numProductions; i++) {
        string production;
        cin >> production;
        grammar.push_back(production);
    }
    
    for (const auto& prod : grammar) {
        size_t equalPos = prod.find('=');
        if (equalPos == string::npos) {
            terminateOnInvalidGrammar();
        }
        bool valid = true;
        for (size_t j = equalPos + 1; j < prod.size(); j++) {
            char currentChar = prod[j];
            if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '(' || currentChar == ')') {
                continue;
            } else if (isupper(currentChar) || islower(currentChar)) {
                continue;
            } else {
                valid = false;
                break;
            }
        }
        if (!valid) {
            terminateOnInvalidGrammar();
        }
    }

    cout << "Operator grammar" << endl;

    return 0;
}