#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <vector>
#include <map>
#include <string>

using namespace std;

class grammar
{
public:
    vector<string> nonterminal;
    vector<string> terminal;
    string startsymbol;
    map<string, vector<vector<string>>> rules;
    grammar(/* args */);
    ~grammar();
};


#endif