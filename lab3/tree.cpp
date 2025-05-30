#include "tree.hpp"

Tree::Tree(Lexer &_lex) : lex(_lex) {
    root = make_shared<Node>();

}

Tree::~Tree() {
}



void Tree::build() {
    lex.start();

    names["programm"] = 0;
    names["block"] = 0;
    names["op_list"] = 0;
    names["operator"] = 0;
    names["operation"] = 0;
    names["tail"] = 0;
    names["identificator"] = 0;
    names["expression"] = 0;
    names["logic_express"] = 0;
    names["unichlen"] = 0;
    names["doublecheln"] = 0;
    names["firstclen"] = 0;
    names["logic_z"] = 0;
    names["logic_oper"] = 0;
    names["eq"] = 0;
    names["AND"] = 0;
    names["OR"] = 0;
    names["NOT"] = 0;

    shared_ptr<Node> item = block();
    root->nodes.push_back(item);
    root->name = get_name("programm"); //TODO
}

shared_ptr<Node> Tree::block() {
    cout << "block\n";
    auto lexem = lex.next();
    if (lexem.type != OP_BLOCK)
        throw __EXCEPTION__; //TODO
    shared_ptr<Node> item = operators_list();
    lexem = lex.next();
    if (lexem.type != CL_BLOCK)
        throw __EXCEPTION__; //TODO
    cout << "close block founded\n";
    shared_ptr<Node> res = make_shared<Node>();
    res->nodes.push_back(item);
    //res->nodes[item] = vector<shared_ptr<Node>>();
    res->name = get_name("block"); //TODO
    cout << "block ended\n";
    return res;
}

shared_ptr<Node> Tree::operators_list() {
    cout << "operators_list\n";
    shared_ptr<Node> item1 = operator_oper();
    if (!item1)
        throw __EXCEPTION__; //TODO
    shared_ptr<Node> item2 = operator_tail();

    shared_ptr<Node> res = make_shared<Node>();
    res->nodes.push_back(item1);
    if (item2)
        res->nodes.push_back(item2);
    res->name = get_name("op_list"); //TODO
    cout << "operators_list ended\n";
    return res;
}

shared_ptr<Node> Tree::operator_tail() {
    cout << "operator_tail\n";
    auto lexem = lex.next();
    cout << "lexe: " << lexem.value << "\n";
    if (lexem.type != SEMYC) {
        lex.prev();
        return nullptr;
    }
    cout << "start oper oper\n";
    shared_ptr<Node> item1 = operator_oper();
    cout << "after oper oper\n";
    if (!item1)
        throw __EXCEPTION__; //TODO
    cout << "new operator_tail\n";
    shared_ptr<Node> item2 = operator_tail();

    shared_ptr<Node> res = make_shared<Node>();
    res->nodes.push_back(item1);
    if (item2)
        res->nodes.push_back(item2);
    res->name = get_name("tail"); //TODO
    cout << "operator_tail ended\n";
    return res;
}

shared_ptr<Node> Tree::operator_oper() {
    cout << "operator_oper\n";
    shared_ptr<Node> item1 = identificator();
    if (!item1)
        throw __EXCEPTION__; //TODO

    auto lexem2 = lex.next();
    cout << "equat: " << lexem2.value << "\n";
    if (lexem2.type != EQUAT)
        throw __EXCEPTION__; //TODO
    shared_ptr<Node> item2 = make_shared<Node>();
    item2->name = get_name("eq");
    item2->is_leaf = true;
    item2->value = lexem2.value;

    auto item3 = expression();
    if (!item3)
        throw __EXCEPTION__; //TODO

    shared_ptr<Node> res = make_shared<Node>();
    res->name = get_name("operator");//TODO
    res->nodes.push_back(item1);
    res->nodes.push_back(item2);
    res->nodes.push_back(item3);
    cout << "operator_oper ended\n";
    return res;
}

shared_ptr<Node> Tree::identificator() {
    cout << "identificator\n";
    auto lexem1 = lex.next();
    cout << "ide: " << lexem1.value << "\n";
    if (lexem1.type != VARYAB)
        return nullptr;
    shared_ptr<Node> item = make_shared<Node>();
    item->name = get_name("identificator");
    item->is_leaf = true;
    item->value = lexem1.value;
    cout << "identificator ended\n";
    return item;
}

shared_ptr<Node> Tree::expression() {
    cout << "expression\n";
    shared_ptr<Node> item = logical_expression();
    cout << "expression ended\n";
    return item;
}

shared_ptr<Node> Tree::logical_expression() {
    cout << "logical_expression\n";
    shared_ptr<Node> item1 = logical_unichlen();
    if (!item1)
        throw __EXCEPTION__; //TODO

    auto lexem = lex.next();
    cout << "leee: " << lexem.value << "\n";

    shared_ptr<Node> item_res = make_shared<Node>();
    item_res->name = get_name("expression");
    item_res->nodes.push_back(item1);

    if (lexem.type != OR_OPER) {
        cout << "not or!!!\n";
        lex.prev();
    }
    else {
        cout << "or hz\n";
        shared_ptr<Node> item_op = make_shared<Node>();
        item_op->name = get_name("OR");
        item_op->value = lexem.value;
        item_res->nodes.push_back(item_op);

        shared_ptr<Node> item2 = logical_expression();
        if (!item2)
            throw __EXCEPTION__; //TODO
        item_res->nodes.push_back(item2);
    }
    cout << "logical_expression ended\n";
    return item_res;
}

shared_ptr<Node> Tree::logical_unichlen() {
    cout << "logical_unichlen\n";
    shared_ptr<Node> item1 = logical_seconded();
    if (!item1)
        throw __EXCEPTION__; //TODO

    auto lexem = lex.next();
    cout << "leee: " << lexem.value << "\n";

    shared_ptr<Node> item_res = make_shared<Node>();
    item_res->name = get_name("logic_express");
    item_res->nodes.push_back(item1);

    if (lexem.type != AND_OPER) {
        cout << "not and!!!\n";
        lex.prev();
    }
    else {
        cout << "and hz\n";
        shared_ptr<Node> item_op = make_shared<Node>();
        item_op->name = get_name("AND");
        item_op->value = lexem.value;
        item_res->nodes.push_back(item_op);

        shared_ptr<Node> item2 = logical_unichlen();
        if (!item2)
            throw __EXCEPTION__; //TODO
        item_res->nodes.push_back(item2);
    }
    cout << "logical_unichlen ended\n";
    return item_res;
}

shared_ptr<Node> Tree::logical_seconded() {
    cout << "logical_seconded\n";
    auto lexem = lex.next();
    cout << "second: " << lexem.value << "\n";
    shared_ptr<Node> item_not;
    if (lexem.type == NOT_OPER) {
        item_not = make_shared<Node>();
        item_not->name = get_name("NOT");
        item_not->value = lexem.value;
    }
    else
        lex.prev();
    shared_ptr<Node> item = logical_firsted();
    if (!item)
        throw __EXCEPTION__; //TODO
    
    shared_ptr<Node> item_res = make_shared<Node>();
    if (item_not)
        item_res->nodes.push_back(item_not);
    item_res->nodes.push_back(item);
    item_res->name = get_name("doublecheln");
    cout << "logical_seconded ended\n";
    return item_res;
}

shared_ptr<Node> Tree::logical_firsted() {
    cout << "logical_firsted\n";
    shared_ptr<Node> item = logical_means();
    if (item) {
        cout << "logical_means founded\n";
        return item;
    }
    cout << "logical_means not founded\n";
    item = identificator();
    return item;
}

shared_ptr<Node> Tree::logical_means() {
    cout << "logical_means\n";
    auto lexem = lex.next();
    if (lexem.type == LOGICAL) {
        shared_ptr<Node> item = make_shared<Node>();
        item->is_leaf = true;
        item->name = get_name("logic_z");
        item->value = lexem.value;
        return item;
    }
    lex.prev();
    return nullptr;
}

string Tree::get_name(string type) {
    names[type]++;
    return type + to_string(names[type]);
}

void Tree::print_tree(ofstream &f) {
    f << "digraph a {\n";
    print_node(f, root);
    f << "}";
    f.close();
}

bool not_cringe(string s) {
    return s != "" && s[0] != '~' && s[0] != '!' && s[0] != '&' && s[0] != '=';
}

void Tree::print_node(ofstream &f, shared_ptr<Node> n) {
    for (int i = 0; i < n->nodes.size(); i++) {
        f << "  \"" << n->name;
        if (not_cringe(n->value)) {
            f << " v " << n->value;
        }
        f << "\" -> \"" << n->nodes[i]->name ;
        if (not_cringe(n->nodes[i]->value)) {
            f << " v " << n->nodes[i]->value;
        }
        f << "\"\n";
        print_node(f, n->nodes[i]);
    }
}