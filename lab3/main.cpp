#include "lexer.hpp"
#include "tree.hpp"

#include <iostream>
#include <sstream>

using namespace std;


/*fine UNARY 1
#define BYNARY 2
#define OP_BLANK 3
#define CL_BLANK 4
#define OP_BLOCK 5
#define CL_BLOCK 6
#define EQUAT 7
#define SEMYC 8
#define LOGICAL 9
#define VARYAB 10
#define OTHER 0*/

string tostrfr(int i) {
    switch (i)
    {
    case AND_OPER:
        return "AND";
        break;
    case OR_OPER:
        return "OR";
        break;
    case NOT_OPER:
        return "NOT";
        break;
    case OP_BLANK:
        return "OP_BLANK";
        break;
    case CL_BLANK:
        return "CL_BLANK";
        break;
    case CL_BLOCK:
        return "CL_BLOCK";
        break;
    case OP_BLOCK:
        return "OP_BLOCK";
        break;
    case EQUAT:
        return "EQUAT";
        break;
    case SEMYC:
        return "SEMYC";
        break;
    case LOGICAL:
        return "LOGICAL";
        break;
    case VARYAB:
        return "VARYAB";
        break;
    case OTHER:
        return "OTHER";
        break;
    default:
        break;
    }
}

int main() {
    //lexer
    //ast_builder
    ifstream file("../aaa.txt");
    stringstream sss;
    sss << file.rdbuf();
    string s = sss.str();
    // string s = R"( {
    //     a = false;
    //     b = true;
    //     c = a ! ~b;
    //     }
    // )";
    //
    //    c = a ! ~b
    //  string s = R"( {
    //     a = false
    //     }
    // )";

    Lexer lex;
    auto res = lex.lex(s);
    for (auto tok: res) {
        cout << tok.value << " " << tostrfr(tok.type) << "\n";
    }
    Tree tree(lex);
    tree.build();
    ofstream f("../a.gv");
    tree.print_tree(f);
}