#ifndef FINCS_HPP
#define FINCS_HPP

#include "grammar.hpp"
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

grammar remove_left_recursion_indirect(grammar g);
grammar remove_left_recursion_immediate(grammar g);
grammar remove_unattainable_symbols(grammar g);
grammar left_factorisation(grammar g);

#endif