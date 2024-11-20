#ifndef SLR_PARSER_H
#define SLR_PARSER_H

#include "grammar.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

class SLRParser {
public:
    Grammar G_prime;
    std::vector<std::pair<std::string, std::vector<std::string>>> G_indexed;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::vector<std::map<std::string, std::set<std::vector<std::string>>>> C;
    std::vector<std::string> action;
    std::vector<std::string> goto_symbols;
    std::vector<std::string> parse_table_symbols;
    std::vector<std::map<std::string, std::string>> parse_table;

    SLRParser(const Grammar &G) : G_prime(augmentGrammar(G)) {
        initializeIndexedGrammar();
        computeFirstFollow();
        C = items(G_prime);
        initializeParseTableSymbols();
        initializeParseTable();
    }

    void parse(const std::string &input);
    void print_info();
    void generate_automaton();
    void print_LR_parser(const std::map<std::string, std::vector<std::string>> &results);

private:
    Grammar augmentGrammar(const Grammar &G);
    void initializeIndexedGrammar();
    void computeFirstFollow();
    std::vector<std::map<std::string, std::set<std::vector<std::string>>>> items(const Grammar &G_prime);
    std::map<std::string, std::set<std::vector<std::string>>> GOTO(
        const std::map<std::string, std::set<std::vector<std::string>>> &I,
        const std::string &X,
        const Grammar &G_prime);
    std::map<std::string, std::set<std::vector<std::string>>> CLOSURE(
        const std::map<std::string, std::set<std::vector<std::string>>> &I,
        const Grammar &G_prime);
    void initializeParseTableSymbols();
    void initializeParseTable();
    std::map<std::string, std::set<std::string>> first_follow(const Grammar &G, std::map<std::string, std::set<std::string>> &follow);
    std::map<std::string, std::vector<std::string>> LR_parser(const std::string &w);
};

#endif // SLR_PARSER_H