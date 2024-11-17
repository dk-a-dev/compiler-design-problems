#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

class Grammar {
public:
    std::string grammar_str;
    std::map<std::string, std::set<std::vector<std::string>>> grammar;
    std::string start;
    std::set<std::string> terminals;
    std::set<std::string> nonterminals;
    std::set<std::string> symbols;

    Grammar(const std::string& grammar_str) {
        this->grammar_str = grammar_str;
        std::istringstream iss(grammar_str);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.empty()) continue;
            std::string head, bodies_str;
            std::istringstream line_stream(line);
            std::getline(line_stream, head, ' ');
            line_stream.ignore(4); // Ignore " -> "
            std::getline(line_stream, bodies_str);

            if (!isupper(head[0])) {
                throw std::invalid_argument("'" + head + " -> " + bodies_str + "': Head '" + head + "' is not capitalized to be treated as a nonterminal.");
            }

            if (start.empty()) {
                start = head;
            }

            grammar[head] = {};
            nonterminals.insert(head);

            std::istringstream bodies_stream(bodies_str);
            std::string body;
            while (std::getline(bodies_stream, body, '|')) {
                std::istringstream body_stream(body);
                std::vector<std::string> body_vec;
                std::string symbol;
                while (body_stream >> symbol) {
                    if (symbol == "^" && body_vec.size() != 1) {
                        throw std::invalid_argument("'" + head + " -> " + body + "': Null symbol '^' is not allowed here.");
                    }
                    body_vec.push_back(symbol);
                    if (!isupper(symbol[0]) && symbol != "^") {
                        terminals.insert(symbol);
                    } else if (isupper(symbol[0])) {
                        nonterminals.insert(symbol);
                    }
                }
                grammar[head].insert(body_vec);
            }
        }
        symbols.insert(terminals.begin(), terminals.end());
        symbols.insert(nonterminals.begin(), nonterminals.end());
    }
};

#endif // GRAMMAR_H