#include <iostream>
#include <sstream>
#include <string>
using namespace std;

void compileToAssembly(const string& inputCode) {
    istringstream iss(inputCode);
    string dest, equals, src1, op, src2;

    // Parse the three-address code
    iss >> dest >> equals >> src1 >> op >> src2;

    // Generate and display the assembly instructions
    cout << "OPCODE\tSRC\tDEST\n";
    cout << "Mov\t" << src1 << "\tr0" << endl;

    if (op == "+") {
        cout << "Add\t#" << src2 << "\tr0" << endl;
    } else if (op == "-") {
        cout << "Sub\t#" << src2 << "\tr0" << endl;
    } else if (op == "*") {
        cout << "Mul\t#" << src2 << "\tr0" << endl;
    } else if (op == "/") {
        cout << "Div\t#" << src2 << "\tr0" << endl;
    } else {
        cerr << "Error: Unsupported operator '" << op << "'" << endl;
        return;
    }

    cout << "Mov\tr0\t" << dest << endl;
}

int main() {
    string inputCode;
    cout << "Enter the three-address code: ";
    getline(cin, inputCode);

    compileToAssembly(inputCode);

    return 0;
}