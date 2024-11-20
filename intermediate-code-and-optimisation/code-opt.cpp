#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct Code {
    string left;  
    string right; 
};

void displayCode(const vector<Code>& code) {
    for (const auto& line : code) {
        cout << line.left << " = " << line.right << endl;
    }
}

vector<Code> eliminateDeadCode(const vector<Code>& code) {
    unordered_map<string, bool> isUsed;
    for (int i = code.size() - 1; i >= 0; --i) {
        isUsed[code[i].left] = false;
    }
    for (const auto& line : code) {
        for (auto& variable : isUsed) {
            if (line.right.find(variable.first) != string::npos) {
                variable.second = true;
            }
        }
    }
    vector<Code> optimizedCode;
    for (const auto& line : code) {
        if (isUsed[line.left] || line.left == code.back().left) {
            optimizedCode.push_back(line);
        }
    }
    return optimizedCode;
}

vector<Code> eliminateCommonExpressions(const vector<Code>& code) {
    unordered_map<string, string> exprMap;
    vector<Code> optimizedCode;
    for (auto line : code) {
        if (exprMap.count(line.right)) {
            line.right = exprMap[line.right];
        } else {
            exprMap[line.right] = line.left;
        }
        optimizedCode.push_back(line);
    }
    return optimizedCode;
}

int main() {
    int n;
    cout << "Enter the Number of Values: ";
    cin >> n;

    vector<Code> code;
    string left, right;

    for (int i = 0; i < n; ++i) {
        cout << "Left: ";
        cin >> left;
        cout << "Right: ";
        cin >> right;
        code.push_back({left, right});
    }

    cout << "\nIntermediate Code:\n";
    displayCode(code);

    auto deadCodeEliminated = eliminateDeadCode(code);
    cout << "\nAfter Dead Code Elimination:\n";
    displayCode(deadCodeEliminated);

    auto optimizedCode = eliminateCommonExpressions(deadCodeEliminated);
    cout << "\nEliminate Common Expression:\n";
    displayCode(optimizedCode);

    cout << "\nOptimized Code:\n";
    displayCode(optimizedCode);

    return 0;
}