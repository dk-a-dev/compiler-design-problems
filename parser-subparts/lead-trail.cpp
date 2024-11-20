#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

vector<Production> grammar;
unordered_map<char, unordered_set<char>> leading;
unordered_map<char, unordered_set<char>> trailing;

void calculateLeading() {
    bool changed;
    do {
        changed = false;
        for (const auto& prod : grammar) {
            char lhs = prod.lhs;
            string rhs = prod.rhs;

            for (char ch : rhs) {
                if (isupper(ch)) {
                    for (char lead : leading[ch]) {
                        if (leading[lhs].insert(lead).second) {
                            changed = true;
                        }
                    }
                } else {
                    if (leading[lhs].insert(ch).second) {
                        changed = true;
                    }
                    break;
                }
            }
        }
    } while (changed);
}

void calculateTrailing() {
    bool changed;
    do {
        changed = false;
        for (const auto& prod : grammar) {
            char lhs = prod.lhs;
            string rhs = prod.rhs;

            for (auto it = rhs.rbegin(); it != rhs.rend(); ++it) {
                char ch = *it;
                if (isupper(ch)) {
                    for (char trail : trailing[ch]) {
                        if (trailing[lhs].insert(trail).second) {
                            changed = true;
                        }
                    }
                } else {
                    if (trailing[lhs].insert(ch).second) {
                        changed = true;
                    }
                    break;
                }
            }
        }
    } while (changed);
}

void printSet(const unordered_map<char, unordered_set<char>>& setMap, const string& setName) {
    for (const auto& entry : setMap) {
        cout << setName << "(" << entry.first << ") = { ";
        for (char ch : entry.second) {
            cout << ch << "";
        }
        cout << "}" << endl;
    }
}

int main() {
    int numProductions;
    cout << "Enter the number of productions: ";
    cin >> numProductions;

    grammar.resize(numProductions);
    cout << "Enter the productions (e.g., E=E+T):" << endl;
    for (int i = 0; i < numProductions; ++i) {
        string production;
        cin >> production;
        grammar[i].lhs = production[0];
        grammar[i].rhs = production.substr(2);
    }

    calculateLeading();
    calculateTrailing();

    cout << "\nLeading sets:" << endl;
    printSet(leading, "Leading");

    cout << "\nTrailing sets:" << endl;
    printSet(trailing, "Trailing");

    return 0;
}