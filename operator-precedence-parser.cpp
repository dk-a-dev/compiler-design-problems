#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <stdexcept>
#include <algorithm>

using namespace std;

bool grammarCheck(int i) {
    cout << "Enter the " << i + 1 << "th grammar(production) want to be checked\nFor null production please enter any special symbol or whitespace..." << endl;
    string input;
    getline(cin, input);
    size_t pos = input.find("->");
    if (pos == string::npos) return false;

    string lhs = input.substr(0, pos);
    string rhs = input.substr(pos + 2);

    if (lhs.empty() || islower(lhs[0])) return false;

    vector<char> f;
    for (char ch = 'a'; ch <= 'z'; ++ch) f.push_back(ch);

    vector<char> s;
    for (char ch = 'A'; ch <= 'Z'; ++ch) s.push_back(ch);

    vector<char> o = {'(', '^', '/', '*', '+', '-', '|', ')'};
    for (char ch = 'a'; ch <= 'z'; ++ch) o.push_back(ch);

    vector<char> sp = {'!', '@', '#', '$', '?', '~', '`', ',', ';', ':', '"', '=', '_', '&', '\'', ' ', '\0'};

    bool g = true;
    for (size_t i = 0; i < rhs.size(); i += 2) {
        if (rhs[i] == ' ') {
            g = false;
        } else if (find(sp.begin(), sp.end(), rhs[i]) != sp.end()) {
            g = false;
            break;
        } else if (rhs.back() == ')' && ((rhs[0] == '(' && rhs.back() == ')') || (count(rhs.begin(), rhs.end(), '(') == count(rhs.begin(), rhs.end(), ')')))) {
            g = true;
        } else if (find(f.begin(), f.end(), rhs[i]) != f.end()) {
            g = true;
        } else if (find(o.begin(), o.end(), rhs.back()) != o.end()) {
            g = false;
        } else if (i == rhs.size() - 1 && find(s.begin(), s.end(), rhs[i]) != s.end()) {
            g = true;
        } else if (i == rhs.size() - 1 && find(s.begin(), s.end(), rhs[i]) == s.end() && find(o.begin(), o.end(), rhs[i]) != o.end() && find(o.begin(), o.end(), rhs[i - 1]) != o.end()) {
            g = true;
        } else if (find(s.begin(), s.end(), rhs[i]) != s.end() && find(o.begin(), o.end(), rhs[i + 1]) != o.end()) {
            g = true;
        } else if (find(s.begin(), s.end(), rhs[i]) != s.end() && find(s.begin(), s.end(), rhs[i + 1]) != s.end()) {
            g = false;
            break;
        } else {
            g = false;
            break;
        }
    }
    return g;
}

bool stringCheck() {
    cout << "Enter the operator used in the given grammar including the terminals\nnon-terminals should be in cursive (small) letters" << endl;
    string input;
    getline(cin, input);
    vector<char> a(input.begin(), input.end());
    a.push_back('$');

    vector<char> l;
    for (char ch = 'a'; ch <= 'z'; ++ch) l.push_back(ch);

    vector<char> o = {'/', '*', '%', '+', '-'};
    vector<char> p = {'/', '*', '%', '+', '-'};

    vector<vector<string> > n(a.size() + 1, vector<string>(a.size() + 1, ""));

    for (size_t j = 1; j < a.size() + 1; ++j) {
        n[0][j] = a[j - 1];
        n[j][0] = a[j - 1];
    }

    for (size_t i = 1; i < a.size() + 1; ++i) {
        for (size_t j = 1; j < a.size() + 1; ++j) {
            if (find(l.begin(), l.end(), n[i][0][0]) != l.end() && find(l.begin(), l.end(), n[0][j][0]) != l.end()) {
                n[i][j] = "";
            } else if (find(l.begin(), l.end(), n[i][0][0]) != l.end()) {
                n[i][j] = ">";
            } else if (find(o.begin(), o.end(), n[i][0][0]) != o.end() && find(o.begin(), o.end(), n[0][j][0]) != o.end()) {
                if (find(o.begin(), o.end(), n[i][0][0]) <= find(o.begin(), o.end(), n[0][j][0])) {
                    n[i][j] = ">";
                } else {
                    n[i][j] = "<";
                }
            } else if (find(o.begin(), o.end(), n[i][0][0]) != o.end() && find(l.begin(), l.end(), n[0][j][0]) != l.end()) {
                n[i][j] = "<";
            } else if (n[i][0] == "$" && n[0][j] != "$") {
                n[i][j] = "<";
            } else if (n[0][j] == "$" && n[i][0] != "$") {
                n[i][j] = ">";
            } else {
                break;
            }
        }
    }

    cout << "The Operator Precedence Relational Table\n=============================================" << endl;
    for (const auto &row : n) {
        for (const auto &elem : row) {
            cout << elem << " ";
        }
        cout << endl;
    }

    cout << "Enter the string you want to check (non-terminals should be in cursive (small) letters):" << endl;
    getline(cin, input);
    vector<char> i(input.begin(), input.end());
    i.push_back('$');

    vector<char> s(i.size(), '\0');
    int q = 0;
    s.insert(s.begin(), '$');

    vector<char> x;
    for (const auto &row : n) {
        x.push_back(row[0][0]);
    }

    vector<char> y;
    for (const auto &elem : n[0]) {
        y.push_back(elem[0]);
    }

    int h = 0;
    while (s[0] != s[1]) {
        if (find(p.begin(), p.end(), i[i.size() - 2]) != p.end()) {
            break;
        } else if (find(x.begin(), x.end(), s[q]) != x.end() && find(y.begin(), y.end(), i[h]) != y.end()) {
            if (n[find(x.begin(), x.end(), s[q]) - x.begin()][find(y.begin(), y.end(), i[h]) - y.begin()] == "<") {
                q++;
                s.insert(s.begin() + q, i[h]);
                h++;
            } else if (n[find(x.begin(), x.end(), s[q]) - x.begin()][find(y.begin(), y.end(), i[h]) - y.begin()] == ">") {
                s.erase(s.begin() + q);
                q--;
            } else if (n[find(x.begin(), x.end(), s[q]) - x.begin()][find(y.begin(), y.end(), i[h]) - y.begin()] == "" && s[q] == '$' && i[h] == '$') {
                s[1] = s[0];
            }
        } else {
            break;
        }
    }

    if (s[0] != s[1]) {
        return false;
    } else {
        return true;
    }
}

int main() {
    int c;
    cout << "Enter the number of LHS variables.." << endl;
    cin >> c;
    cin.ignore();

    bool t = true;
    for (int i = 0; i < c; ++i) {
        if (!grammarCheck(i)) {
            t = false;
            break;
        }
    }

    if (t) {
        cout << "Grammar is accepted" << endl;
        if (stringCheck()) {
            cout << "String is accepted" << endl;
        } else {
            cout << "String is not accepted" << endl;
        }
    } else {
        cout << "Grammar is not accepted" << endl;
    }

    return 0;
}