#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct ThreeAddressCode {
    string result;
    string operand1;
    string op;
    string operand2;
};

void generateThreeAddressCode(vector<ThreeAddressCode>& code, const string& result, const string& operand1, const string& op, const string& operand2) {
    code.push_back({result, operand1, op, operand2});
}

void displayThreeAddressCode(const vector<ThreeAddressCode>& code) {
    for (const auto& line : code) {
        cout << line.result << " = " << line.operand1;
        if (!line.op.empty()) {
            cout << " " << line.op << " " << line.operand2;
        }
        cout << endl;
    }
}

int main() {
    vector<ThreeAddressCode> code;
    int tempCount = 1;
    
    string t1 = "t" + to_string(tempCount++);
    generateThreeAddressCode(code, t1, "b", "*", "c");

    string t2 = "t" + to_string(tempCount++);
    generateThreeAddressCode(code, t2, t1, "+", "9");

    generateThreeAddressCode(code, "a", t2, "", "");

    string t3 = "t" + to_string(tempCount++);
    generateThreeAddressCode(code, t3, "b", "*", "c");

    string t4 = "t" + to_string(tempCount++);
    generateThreeAddressCode(code, t4, t3, "*", "e");

    generateThreeAddressCode(code, "d", t4, "", "");

    cout << "Three-Address Code:\n";
    displayThreeAddressCode(code);

    return 0;
}