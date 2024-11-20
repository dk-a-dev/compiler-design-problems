#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_SCOPE_DEPTH 10

typedef struct {
    char name[50];
    char type[50];
    char scope[50];
    int address;
    // int used;
    int lineNumber;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int currentAddress = 1000;

void addSymbol(const char *name, const char *type, const char *scope, int lineNumber) {
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    strcpy(symbolTable[symbolCount].scope, scope);
    symbolTable[symbolCount].address = currentAddress;
    // symbolTable[symbolCount].used = used;
    symbolTable[symbolCount].lineNumber = lineNumber;
    currentAddress += 1000;
    symbolCount++;
}

// void updateUsage(const char *name, const char *scope) {
//     for (int i = 0; i < symbolCount; i++) {
//         if (strcmp(symbolTable[i].name, name) == 0 && strcmp(symbolTable[i].scope, scope) == 0) {
//             symbolTable[i].used = 1;
//         }
//     }
// }

int main() {
    FILE *file = fopen("Input.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[256];
    char scopeStack[MAX_SCOPE_DEPTH][50];
    int scopeDepth = 0;
    int lineNumber = 0;
    strcpy(scopeStack[scopeDepth], "global");

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;

        char *token = strtok(line, " \t\n;(){}=+-*/");

        while (token != NULL) {
            if (strcmp(token, "int") == 0) {
                token = strtok(NULL, " \t\n;(){}=+-*/");
                if (token != NULL && isalpha(token[0])) {
                    addSymbol(token, "int", scopeStack[scopeDepth], lineNumber);
                }
            } else if (strcmp(token, "main") == 0) {
                scopeDepth++;
                strcpy(scopeStack[scopeDepth], "main");
            } else if (strcmp(token, "if") == 0) {
                scopeDepth++;
                strcpy(scopeStack[scopeDepth], "if");
            } else if (strcmp(token, "}") == 0) {
                if (scopeDepth > 0) {
                    scopeDepth--;
                }
            }
            // } else {
            //     for (int i = 0; i < symbolCount; i++) {
            //         if (strcmp(symbolTable[i].name, token) == 0) {
            //             updateUsage(token, scopeStack[scopeDepth]);
            //         }
            //     }
            // }
            token = strtok(NULL, " \t\n;(){}=+-*/");
        }
    }

    fclose(file);

    printf("Scope | Symbol Name | Type | Address | Line Number\n");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s | %s | %s | %d | %d\n",
               symbolTable[i].scope,
               symbolTable[i].name,
               symbolTable[i].type,
               symbolTable[i].address,
               symbolTable[i].lineNumber);
    }

    return 0;
}