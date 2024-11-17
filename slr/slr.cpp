#include "slr_parser.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

Grammar SLRParser::augmentGrammar(const Grammar &G) {
    Grammar G_prime = G;
    G_prime.start = G.start + "'";
    G_prime.grammar[G_prime.start] = {{G.start}};
    G_prime.nonterminals.insert(G_prime.start);
    G_prime.symbols.insert(G_prime.start);
    return G_prime;
}

void SLRParser::initializeIndexedGrammar() {
    for (const auto &prod : G_prime.grammar) {
        for (const auto &body : prod.second) {
            G_indexed.push_back({prod.first, body});
        }
    }
}

void SLRParser::computeFirstFollow() {
    auto union_sets = [](std::set<std::string> &set1, const std::set<std::string> &set2) {
        size_t initial_size = set1.size();
        set1.insert(set2.begin(), set2.end());
        return set1.size() != initial_size;
    };

    first = {G_prime.symbols.begin(), G_prime.symbols.end()};
    for (const auto &terminal : G_prime.terminals) {
        first[terminal] = {terminal};
    }
    follow = {G_prime.nonterminals.begin(), G_prime.nonterminals.end()};
    follow[G_prime.start].insert("$");

    bool updated;
    do {
        updated = false;
        for (const auto &prod : G_prime.grammar) {
            const std::string &head = prod.first;
            for (const auto &body : prod.second) {
                for (size_t i = 0; i < body.size(); ++i) {
                    const std::string &symbol = body[i];
                    if (symbol != "^") {
                        updated |= union_sets(first[head], first[symbol]);
                        if (first[symbol].find("^") == first[symbol].end()) {
                            break;
                        }
                    } else {
                        updated |= union_sets(first[head], {"^"});
                    }
                }
                std::set<std::string> aux = follow[head];
                for (auto it = body.rbegin(); it != body.rend(); ++it) {
                    const std::string &symbol = *it;
                    if (symbol != "^") {
                        if (G_prime.nonterminals.find(symbol) != G_prime.nonterminals.end()) {
                            updated |= union_sets(follow[symbol], aux);
                        }
                        if (first[symbol].find("^") != first[symbol].end()) {
                            aux.insert(first[symbol].begin(), first[symbol].end());
                            aux.erase("^");
                        } else {
                            aux = first[symbol];
                        }
                    }
                }
            }
        }
    } while (updated);
}

std::vector<std::map<std::string, std::set<std::vector<std::string>>>> SLRParser::items(const Grammar &G_prime) {
    std::vector<std::map<std::string, std::set<std::vector<std::string>>>> C;
    std::map<std::string, std::set<std::vector<std::string>>> I0 = {{G_prime.start, {{"." , G_prime.start.substr(0, G_prime.start.size() - 1)}}}};
    I0 = CLOSURE(I0, G_prime);
    C.push_back(I0);

    bool updated;
    do {
        updated = false;
        for (const auto& I : C) {
            for (const auto& X : G_prime.symbols) {
                auto goto_I_X = GOTO(I, X, G_prime);
                if (!goto_I_X.empty() && std::find(C.begin(), C.end(), goto_I_X) == C.end()) {
                    C.push_back(goto_I_X);
                    updated = true;
                }
            }
        }
    } while (updated);

    return C;
}

std::map<std::string, std::set<std::vector<std::string>>> SLRParser::GOTO(
    const std::map<std::string, std::set<std::vector<std::string>>> &I,
    const std::string &X,
    const Grammar &G_prime) {
    std::map<std::string, std::set<std::vector<std::string>>> J;
    for (const auto& [A, items] : I) {
        for (const auto& item : items) {
            auto it = std::find(item.begin(), item.end(), ".");
            if (it != item.end() && it + 1 != item.end() && *(it + 1) == X) {
                std::vector<std::string> new_item = item;
                std::swap(new_item[it - item.begin()], new_item[it - item.begin() + 1]);
                J[A].insert(new_item);
            }
        }
    }
    return CLOSURE(J, G_prime);
}

std::map<std::string, std::set<std::vector<std::string>>> SLRParser::CLOSURE(
    const std::map<std::string, std::set<std::vector<std::string>>> &I,
    const Grammar &G_prime) {
    std::map<std::string, std::set<std::vector<std::string>>> J = I;
    bool added;
    do {
        added = false;
        std::map<std::string, std::set<std::vector<std::string>>> new_items;
        for (const auto& [A, items] : J) {
            for (const auto& item : items) {
                auto it = std::find(item.begin(), item.end(), ".");
                if (it != item.end() && it + 1 != item.end()) {
                    const std::string& B = *(it + 1);
                    if (G_prime.nonterminals.find(B) != G_prime.nonterminals.end()) {
                        for (const auto& prod : G_prime.grammar.at(B)) {
                            std::vector<std::string> new_item = {"."};
                            new_item.insert(new_item.end(), prod.begin(), prod.end());
                            if (J[B].find(new_item) == J[B].end()) {
                                new_items[B].insert(new_item);
                                added = true;
                            }
                        }
                    }
                }
            }
        }
        for (const auto& [B, items] : new_items) {
            J[B].insert(items.begin(), items.end());
        }
    } while (added);
    return J;
}

void SLRParser::initializeParseTableSymbols() {
    std::vector<std::string> terminals;
    std::copy(G_prime.terminals.begin(), G_prime.terminals.end(), std::back_inserter(terminals));
    parse_table_symbols.insert(parse_table_symbols.end(), terminals.begin(), terminals.end());
    parse_table_symbols.push_back("$");
    std::vector<std::string> nonterminals(G_prime.nonterminals.begin(), G_prime.nonterminals.end());
    parse_table_symbols.insert(parse_table_symbols.end(), nonterminals.begin(), nonterminals.end());
    parse_table_symbols.erase(std::remove(parse_table_symbols.begin(), parse_table_symbols.end(), G_prime.start), parse_table_symbols.end());
}

void SLRParser::initializeParseTable() {
    parse_table.resize(C.size());
    for (size_t i = 0; i < C.size(); ++i) {
        for (const auto& [A, items] : C[i]) {
            for (const auto& item : items) {
                if (item.back() == ".") {
                    if (A == G_prime.start) {
                        parse_table[i]["$"] = "acc";
                    } else {
                        for (const auto& f : follow[A]) {
                            parse_table[i][f] = "r" + std::to_string(std::distance(G_indexed.begin(), std::find(G_indexed.begin(), G_indexed.end(), std::make_pair(A, std::vector<std::string>(item.begin(), item.end() - 1)))));
                        }
                    }
                } else {
                    auto it = std::find(item.begin(), item.end(), ".");
                    if (it != item.end() && it + 1 != item.end()) {
                        const std::string& symbol = *(it + 1);
                        auto goto_state = GOTO(C[i], symbol, G_prime);
                        if (!goto_state.empty()) {
                            int state = std::distance(C.begin(), std::find(C.begin(), C.end(), goto_state));
                            if (G_prime.terminals.find(symbol) != G_prime.terminals.end()) {
                                parse_table[i][symbol] = "s" + std::to_string(state);
                            } else {
                                parse_table[i][symbol] = std::to_string(state);
                            }
                        }
                    }
                }
            }
        }
    }
}

void SLRParser::parse(const std::string &input) {
    std::stack<int> state_stack;
    std::stack<std::string> symbol_stack;
    state_stack.push(0);
    size_t index = 0;
    while (true) {
        int state = state_stack.top();
        std::string symbol(1, input[index]);
        if (parse_table[state][symbol].empty()) {
            std::cout << "Error: Unexpected symbol " << symbol << " at position " << index << std::endl;
            return;
        }
        std::string action = parse_table[state][symbol];
        if (action[0] == 's') {
            int next_state = std::stoi(action.substr(1));
            state_stack.push(next_state);
            symbol_stack.push(symbol);
            ++index;
        } else if (action[0] == 'r') {
            int prod_index = std::stoi(action.substr(1));
            const std::string &head = G_indexed[prod_index].first;
            const std::vector<std::string> &body = G_indexed[prod_index].second;
            for (size_t i = 0; i < body.size(); ++i) {
                state_stack.pop();
                symbol_stack.pop();
            }
            state = state_stack.top();
            state_stack.push(std::stoi(parse_table[state][head]));
            symbol_stack.push(head);
        } else if (action == "acc") {
            std::cout << "Input string is accepted." << std::endl;
            return;
        }
    }
}

void SLRParser::print_info() {
    std::cout << "AUGMENTED GRAMMAR:" << std::endl;
    for (size_t i = 0; i < G_indexed.size(); ++i) {
        std::cout << i << ": " << G_indexed[i].first << " -> ";
        for (const auto &symbol : G_indexed[i].second) {
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "TERMINALS: ";
    for (const auto& terminal : G_prime.terminals) {
        std::cout << terminal << " ";
    }
    std::cout << std::endl;

    std::cout << "NON-TERMINALS: ";
    for (const auto& nonterminal : G_prime.nonterminals) {
        std::cout << nonterminal << " ";
    }
    std::cout << std::endl;

    std::cout << "START SYMBOL: " << G_prime.start << std::endl;
    std::cout << std::endl;

    std::cout << "PRODUCTIONS:" << std::endl;
    for (const auto& prod : G_prime.grammar) {
        std::cout << prod.first << " -> ";
        for (const auto& body : prod.second) {
            for (const auto& symbol : body) {
                std::cout << symbol << " ";
            }
            std::cout << "| ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "CANONICAL COLLECTION OF LR(0) ITEMS:" << std::endl;
    for (size_t i = 0; i < C.size(); ++i) {
        std::cout << "I" << i << ":" << std::endl;
        for (const auto& [A, items] : C[i]) {
            for (const auto& item : items) {
                std::cout << A << " -> ";
                for (const auto& symbol : item) {
                    std::cout << symbol << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}


void SLRParser::print_LR_parser(const std::map<std::string, std::vector<std::string>> &results) {
    std::cout << "SLR Parsing Table:" << std::endl;

    // Print header
    std::cout << std::setw(10) << "State";
    for (const auto &symbol : parse_table_symbols) {
        std::cout << std::setw(10) << symbol;
    }
    std::cout << std::endl;

    // Print table
    for (size_t i = 0; i < parse_table.size(); ++i) {
        std::cout << std::setw(10) << i;
        for (const auto &symbol : parse_table_symbols) {
            if (parse_table[i].find(symbol) != parse_table[i].end()) {
                std::cout << std::setw(10) << parse_table[i][symbol];
            } else {
                std::cout << std::setw(10) << "";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string grammar_str = "E -> E + T | T\nT -> T * F | F\nF -> ( E ) | id";
    Grammar G(grammar_str);

    SLRParser parser(G);
    parser.print_info(); // Print grammar and canonical collection information

    std::string input = "id+id*id$"; // Example input string
    parser.parse(input);

    return 0;
}