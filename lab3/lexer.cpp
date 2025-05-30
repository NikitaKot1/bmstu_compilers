#include "lexer.hpp"

Lexer::Lexer() {
    and_like_opers.push_back("&");
    or_like_opers.push_back("!");
    not_like_opers.push_back("~");
    open_blank = {"("};
    close_blank = {")"};
    semicolon = {";"};
    equating = {"="};
    start_block = {"{"};
    end_block = {"}"};
    logical_z.push_back("true");
    logical_z.push_back("false");
}

Lexer::~Lexer() {}


bool Lexer::is_operator(string t) {
    return is_and_like_operator(t) || is_or_like_operator(t) ||
            is_not_like_operator(t) ||
            is_open_blank(t) || is_close_blank(t) ||
            is_open_block(t) || is_close_block(t) ||
            is_semicolon(t) || is_equating(t);
}

bool Lexer::is_and_like_operator(string t) {
    return find(and_like_opers.begin(), and_like_opers.end(), t) != and_like_opers.end();
}

bool Lexer::is_or_like_operator(string t) {
    return find(or_like_opers.begin(), or_like_opers.end(), t) != or_like_opers.end();
}

bool Lexer::is_not_like_operator(string t) {
    return find(not_like_opers.begin(), not_like_opers.end(), t) != not_like_opers.end();
}

bool Lexer::is_logical(string t) {
    return find(logical_z.begin(), logical_z.end(), t) != logical_z.end();
}


bool Lexer::is_open_blank(string t) {
    return t == open_blank;
}

bool Lexer::is_close_blank(string t) {
    return t == close_blank;
}

bool Lexer::is_open_block(string t) {
    return t == start_block;
}

bool Lexer::is_close_block(string t) {
    return t == end_block;
}

bool Lexer::is_semicolon(string t) {
    return t == semicolon;
}

bool Lexer::is_equating(string t) {
    return t == equating;
}


string Lexer::exract_logical(const string &s, int p) {
    for (auto log: logical_z) {
        string tr_log = "";
        for (int i = 0; i < log.size(); i++)
            tr_log += s[p+i];
        if (is_logical(tr_log))
            return tr_log;
    }
    return "";
}

bool Lexer::is_letter(char c) {
    return isalpha(c);
}

bool Lexer::is_number(char c) {
    return c >= '0' && c <= '9';
}

string Lexer::extract_word(const string &source, int pos) {
    string word = "";
    for (int i = pos; i < source.size(); i++) {
        if (is_letter(source[i]) || is_number(source[i]))
            word += source[i];
        else break;
    }
    return word;
}

vector<token> Lexer::lex(string source) {
    pos = 0;
    tokens.clear();
    for (int i = 0; i < source.size(); i++) {
        char curr = source[i];

        if (curr == '\n')
            continue;

        if (is_operator({curr})) {
            token t; t.value = {curr};
            if (is_and_like_operator({curr}))
                t.type = AND_OPER;
            else if (is_or_like_operator({curr}))
                t.type = OR_OPER;
            else if (is_not_like_operator({curr}))
                t.type = NOT_OPER;
            else if (is_open_blank({curr}))
                t.type = OP_BLANK;
            else if (is_close_blank({curr}))
                t.type = CL_BLANK;
            else if (is_open_block({curr}))
                t.type = OP_BLOCK;
            else if (is_close_block({curr}))
                t.type = CL_BLOCK;
            else if (is_semicolon({curr}))
                t.type = SEMYC;
            else if (is_equating({curr}))
                t.type = EQUAT;
            tokens.push_back(t);
        }

        if (curr == 't' || curr == 'f') { // ну как бэ лучше как-то в общем дело сделать
            string tr_log = exract_logical(source, i);
            if (tr_log != "") {
                token t{tr_log, LOGICAL};
                tokens.push_back(t);
                i += tr_log.size()-1;
                continue;
            }
        }

        if (is_letter(curr)) {
            string word = extract_word(source, i);
            token t{word, VARYAB};
            tokens.push_back(t);
            i += word.size();
            continue;
        }
    }
    return tokens;
}

void Lexer::start() {
    pos = 0;
}

token Lexer::next() {
    // cout << pos << "\n";
    // cout << tokens[pos].value << "\n";
    if (pos >= tokens.size())
        return {"", OTHER};
    assert(pos < tokens.size());
    return tokens[pos++];
}

void Lexer::prev() {
    // cout << pos << "\n";
    // cout << tokens[pos].value << "\n";
    if (pos <= 0)
        return;
    pos--;
}