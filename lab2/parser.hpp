#ifndef PARDER_HPP
#define PARSER_HPP

#include "grammar.hpp"
#include <fstream>
#include <sstream>

using namespace std;

grammar parse(ifstream &f);
void print_grammar(ofstream &f, grammar g);

#endif