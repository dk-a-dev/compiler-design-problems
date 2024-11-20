#include <iostream>
#include <vector>
#include <string>
using namespace std;

void terminateOnInvalidGrammar() {
    cout << "Invalid grammar" << endl;
    exit(1);
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
                if (j + 1 < prod.size() && (isupper(prod[j + 1]) || islower(prod[j + 1]))) {
                    valid = false;
                    break;
                }
                continue;
            } else if (currentChar == '#') {
                valid = false;
                break;
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