#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct DAGNode {
    int serialNumber;
    char value;
    int leftChild;
    int rightChild;

    DAGNode(int sn, char val, int left = -1, int right = -1)
        : serialNumber(sn), value(val), leftChild(left), rightChild(right) {}
};

vector<DAGNode> dag;

void printDAG() {
    for (const auto& node : dag) {
        cout << node.serialNumber << ", " << node.value << ", ";
        if (node.leftChild != -1) {
            cout << "(" << dag[node.leftChild].value << ", " << node.leftChild << ")";
        } else {
            cout << "null";
        }
        cout << ", ";
        if (node.rightChild != -1) {
            cout << "(" << dag[node.rightChild].value << ", " << node.rightChild << ")";
        } else {
            cout << "null";
        }
        cout << endl;
    }
}

void constructDAG(const string& expr) {
    unordered_map<string, int> nodeMap;
    int serialNumber = 0;


    auto addNode = [&](const string& subExpr, char value, int left = -1, int right = -1) {
        if (nodeMap.find(subExpr) == nodeMap.end()) {
            nodeMap[subExpr] = serialNumber++;
            dag.push_back(DAGNode(nodeMap[subExpr], value, left, right));
        }
        return nodeMap[subExpr];
    };

    int a = addNode("a", 'a');
    int b = addNode("b", 'b');
    int c = addNode("c", 'c');
    int a_plus_b = addNode("a+b", '+', a, b);
    int a_plus_b_plus_c = addNode("a+b+c", '+', a_plus_b, c);
    addNode("(a+b)*(a+b+c)", '*', a_plus_b, a_plus_b_plus_c);
}
int main() {
    string expression = "(a+b)*(a+b+c)";
    constructDAG(expression);
    printDAG();
    return 0;
}