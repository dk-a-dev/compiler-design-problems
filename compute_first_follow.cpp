#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cctype>

using namespace std;

unordered_map<char, unordered_set<char>> firstSets;
unordered_map<char, unordered_set<char>> followSets;
vector<string> productions;
int count;

void findFirst(char c);
void findFollow(char c);
void addFollow(char c, char follow);

int main() {
    cout << "Enter the number of productions: ";
    cin >> count;

    cout << "Enter the productions (e.g., E=E+T):\n";
    productions.resize(count);
    for (int i = 0; i < count; ++i) {
        cin >> productions[i];
    }

    for (const auto& prod : productions) {
        findFirst(prod[0]);
    }

    followSets[productions[0][0]].insert('$');
    for (const auto& prod : productions) {
        findFollow(prod[0]);
    }

    cout << "\nFirst Sets:\n";
    for (const auto& entry : firstSets) {
        cout << "First(" << entry.first << ") = { ";
        for (char c : entry.second) {
            cout << c << " ";
        }
        cout << "}\n";
    }

    cout << "\nFollow Sets:\n";
    for (const auto& entry : followSets) {
        cout << "Follow(" << entry.first << ") = { ";
        for (char c : entry.second) {
            cout << c << " ";
        }
        cout << "}\n";
    }

    return 0;
}

void findFirst(char c) {
    if (!isupper(c)) {
        firstSets[c].insert(c);
        return;
    }

    for (const auto& prod : productions) {
        if (prod[0] == c) {
            if (prod[2] == '#') {
                firstSets[c].insert('#');
            } else if (!isupper(prod[2])) {
                firstSets[c].insert(prod[2]);
            } else {
                findFirst(prod[2]);
                for (char first : firstSets[prod[2]]) {
                    if (first != '#') {
                        firstSets[c].insert(first);
                    }
                }
            }
        }
    }
}

void findFollow(char c) {
    for (const auto& prod : productions) {
        for (size_t i = 2; i < prod.size(); ++i) {
            if (prod[i] == c) {
                if (i + 1 < prod.size()) {
                    if (!isupper(prod[i + 1])) {
                        followSets[c].insert(prod[i + 1]);
                    } else {
                        for (char first : firstSets[prod[i + 1]]) {
                            if (first != '#') {
                                followSets[c].insert(first);
                            }
                        }
                        if (firstSets[prod[i + 1]].count('#')) {
                            addFollow(c, prod[0]);
                        }
                    }
                } else {
                    addFollow(c, prod[0]);
                }
            }
        }
    }
}

void addFollow(char c, char follow) {
    if (c != follow) {
        for (char f : followSets[follow]) {
            followSets[c].insert(f);
        }
    }
}