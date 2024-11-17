#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

vector<Production> grammar;
string input;
vector<char> parseStack;

void printStackAndInput(const string& action) {
    cout << "$";
    for (char ch : parseStack) {
        cout << ch;
    }
    cout << "\t\t" << input << "$\t\t" << action << endl;
}

void reduce() {
    bool reduced;
    do {
        reduced = false;
        for (const auto& prod : grammar) {
            string rhs = prod.rhs;
            if (parseStack.size() >= rhs.size()) {
                string topOfStack(parseStack.end() - rhs.size(), parseStack.end());
                if (topOfStack == rhs) {
                    parseStack.erase(parseStack.end() - rhs.size(), parseStack.end());
                    parseStack.push_back(prod.lhs);
                    printStackAndInput("REDUCE TO " + string(1, prod.lhs) + " -> " + rhs);
                    reduced = true;
                    break;
                }
            }
        }
    } while (reduced);
}

int main() {
    int numProductions;
    cout << "Enter the number of productions: ";
    cin >> numProductions;

    grammar.resize(numProductions);
    cout << "Enter the productions (e.g., E=E+T):\n";
    for (int i = 0; i < numProductions; ++i) {
        string production;
        cin >> production;
        grammar[i].lhs = production[0];
        grammar[i].rhs = production.substr(2);
    }

    cout << "Enter the input string: ";
    cin >> input;

    cout << "\nstack \t input \t action" << endl;
    printStackAndInput("SHIFT");

    while (!input.empty()) {
        parseStack.push_back(input[0]);
        input.erase(input.begin());
        printStackAndInput("SHIFT");
        reduce();
    }

    reduce();

    if (parseStack.size() == 1 && parseStack[0] == grammar[0].lhs) {
        cout << "Accept" << endl;
    } else {
        cout << "Reject" << endl;
    }

    return 0;
}