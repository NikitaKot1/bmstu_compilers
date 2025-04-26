#include "grammar.hpp"
#include "parser.hpp"
#include "fincs.hpp"
#include <fstream>
#include <iostream>



int main() {
    ifstream f("../gr.txt");
    grammar g = parse(f);
    ofstream f02("../gr20.txt");
    print_grammar(f02, g);
    grammar g2 = remove_left_recursion_indirect(g);
    grammar g3 = remove_unattainable_symbols(g);
    ofstream f2("../gr2.txt");
    print_grammar(f2, g2);
    f2.close();
    ofstream f3("../gr3.txt");
    print_grammar(f3, g3);
    f3.close();
    g3 = left_factorisation(g3);
    ofstream f4("../gr4.txt");
    print_grammar(f4, g3);
    f4.close();
    return 0;
}