#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <iostream>

using namespace std;

#define NOT_OPER 1
#define AND_OPER 2
#define OR_OPER 3
#define OP_BLANK 4
#define CL_BLANK 5
#define OP_BLOCK 6
#define CL_BLOCK 7
#define EQUAT 8
#define SEMYC 9
#define LOGICAL 10
#define VARYAB 11
#define OTHER 0

struct token
{
    string value;
    int type;
    bool operator==(token &t) {
        return t.value == value;
    }
};


class Lexer {
public:
    Lexer();
    ~Lexer();
protected:
    vector<string> and_like_opers;
    vector<string> or_like_opers;
    vector<string> not_like_opers;
    vector<string> logical_z;
    string open_blank;
    string close_blank;
    string semicolon;
    string equating;
    string start_block;
    string end_block;

private:
    bool is_operator(string t);
    bool is_and_like_operator(string t);
    bool is_or_like_operator(string t);
    bool is_not_like_operator(string t);
    bool is_logical(string t);
    bool is_open_blank(string t);
    bool is_close_blank(string t);
    bool is_open_block(string t);
    bool is_close_block(string t);
    bool is_semicolon(string t);
    bool is_equating(string t);
    bool is_letter(char c);
    bool is_number(char c);

    string exract_logical(const string &source, int pos);
    string extract_word(const string &source, int pos);

    int pos;
    vector<token> tokens;
public:
    vector<token> lex(string source);
    void start();
    token next();
    void prev();
};

#endif