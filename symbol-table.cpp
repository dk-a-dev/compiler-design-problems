#include <iostream>
#include <vector>

using namespace std;

union VarValue {
    int intValue;
    float floatValue;
    char strValue[100];

    VarValue() {}
    VarValue(int val) : intValue(val) {}
    VarValue(float val) : floatValue(val) {}
    VarValue(const char* val) { strcpy(strValue, val); }
    ~VarValue() {}
};

class Headers{
    public:
        enum type{
            integer,floats,str
        }; 

        vector<string> varName;
        vector<type> varType;
        vector<int> address;
        vector<VarValue>value;    
    
        Headers(string varName,type varType,int address,VarValue value){
            this->varName.push_back(varName);
            this->varType.push_back(varType);
            this->address.push_back(address);
            this->value.push_back(value);
        };
};

class SymbolTable{
    private:
        vector<Headers> headers;

    public:
        void insert(string varName, Headers::type varType, int address, VarValue value){
            switch(varType) {
            case Headers::integer:
                if (typeid(value.intValue) != typeid(int)) {
                    cout << "Error: Type mismatch. Expected an integer value." << endl;
                    return;
                }
                break;
            case Headers::floats:
                if (typeid(value.floatValue) != typeid(float)) {
                    cout << "Error: Type mismatch. Expected a float value." << endl;
                    return;
                }
                break;
            case Headers::str:
                break;
            default:
                cout << "Error: Unknown type." << endl;
                return;
            }
            Headers header(varName, varType, address, value);
            headers.push_back(header);
        }

        Headers* search(string varName){
            for(auto& header : headers){
                if(header.varName[0] == varName){
                    checkResult(&header);
                    return &header;
                }
            }
            checkResult(nullptr);
            return nullptr;
        }

        void remove(string varName){
            for(auto it = headers.begin(); it != headers.end(); ++it){
                if(it->varName[0] == varName){
                    headers.erase(it);
                    break;
                }
            }
        }

        void checkResult(Headers* result){
            if(result != nullptr){
                cout << "Variable found!" << endl;
                cout << "Variable Name: " << result->varName[0] << endl;
                cout << "Variable Type: " << result->varType[0] << endl;
                cout << "Variable Address: " << result->address[0] << endl;
                cout << "Variable Value: ";
                switch (result->varType[0]) {
                    case Headers::integer:
                        cout << result->value[0].intValue;
                        break;
                    case Headers::floats:
                        cout << result->value[0].floatValue;
                        break;
                    case Headers::str:
                        cout << result->value[0].strValue;
                        break;
                }
                cout << endl;
            }
            else{
                cout << "Variable not found!" << endl;
            }
        }
};

int main(int argc, char const *argv[])
{
    SymbolTable symbolTable;
    
    symbolTable.insert("x", Headers::integer, 0, 10);
    symbolTable.insert("y", Headers::floats, 4, 3.14f);
    symbolTable.insert("name", Headers::str, 8, "John Doe");
    
    Headers* result = symbolTable.search("y");
    
    
    symbolTable.remove("y");    
    return 0;
}
