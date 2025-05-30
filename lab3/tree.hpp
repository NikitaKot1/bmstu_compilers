#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "lexer.hpp"

using namespace std;

struct Node {
    string value;
    //map<shared_ptr<Node>, vector<shared_ptr<Node>>> nodes;
    vector<shared_ptr<Node>> nodes;
    bool is_leaf;
    string name;
};

class Tree
{
private:
    shared_ptr<Node> root;
    Lexer &lex;
    map<string, int> names;
public:
    Tree(Lexer &_lex);
    ~Tree();

    void build();
    void print_tree(ofstream &f);
protected:
    string get_name(string type);

    shared_ptr<Node> block();
    shared_ptr<Node> operators_list();
    shared_ptr<Node> operator_oper();
    shared_ptr<Node> operator_tail();
    shared_ptr<Node> identificator();
    shared_ptr<Node> expression();
    shared_ptr<Node> logical_expression();
    shared_ptr<Node> logical_unichlen();
    shared_ptr<Node> logical_seconded();
    shared_ptr<Node> logical_firsted();
    shared_ptr<Node> logical_means();

    void print_node(ofstream &f, shared_ptr<Node> n);
};


#endif