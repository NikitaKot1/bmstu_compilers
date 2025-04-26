#include "parser.hpp"

#include <iostream>

grammar parse(ifstream &f) {
    grammar g;
    vector<string> nonterminal;
    vector<string> terminal;
    string startsymbol;
    map<string, vector<vector<string>>>  rules;

    int n_nont = 0;
    f >> n_nont;
    for (int i = 0; i < n_nont; i++) {
        string k; f >> k;
        nonterminal.push_back(k);
    }
    int n_t = 0;
    f >> n_t;
    for (int i = 0; i < n_t; i++) {
        string k; f >> k;
        terminal.push_back(k);
    }
    int laws_n = 0;
    f >> laws_n;
    string line;
    getline(f, line);
    for (int i = 0; i < laws_n; i++) {
        string line;
        getline(f, line);
        istringstream ss(line);
        pair<string, vector<vector<string>>> rule;

        ss >> rule.first;
        string arrow; ss >> arrow;
        
        string alter_s;
        vector<string> alter_rule;
        while (ss >> alter_s) {
            if (alter_s=="|") {
                rule.second.push_back(alter_rule);
                alter_rule.clear();
            }
            else
                alter_rule.push_back(alter_s);
        }
        if (!alter_rule.empty())
            rule.second.push_back(alter_rule);
        //rules.push_back(rule); 
        rules[rule.first] = rule.second;
    }
    string start;
    f >> start;
    startsymbol = start;

    g.nonterminal = nonterminal;
    g.terminal = terminal;
    g.startsymbol = startsymbol;
    g.rules = rules;

    return g;
}

void print_grammar(ofstream &f, grammar g) {
    f << g.nonterminal.size() << "\n";
    for (auto n: g.nonterminal) {
        f << n << " ";
    }
    f << "\n";
    f << g.terminal.size() << "\n";
    for (auto n: g.terminal) {
        f << n << " ";
    }
    f << "\n";

    f << g.rules.size() << "\n";
    for (auto rule: g.rules) {
        f << rule.first << " ->";
        for (int i = 0; i < rule.second.size(); i++) {
            for (auto t: rule.second[i])
                f << " " << t;
            if (i != rule.second.size()-1)
                f << " |";
        }
        f << "\n";
    }
    f << g.startsymbol;
}