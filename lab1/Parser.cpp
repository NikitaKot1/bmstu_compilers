#include "Parser.hpp"
#include <stack>
#include <iostream>
#include <fstream>


Parser::Parser(std::string s) : reg(s), makedAutomata(false), postfix("")
{
}


Parser::~Parser()
{
}

bool Parser::match(std::string s) {
	if (!makedAutomata)
		makeAutomata();
	return dfa.match(s);
}

int priority(char c) {
	if (c == '(') {
		return 1;
	}
	else if (c == '|') {
		return 2;
	}
	else if (c == '.')
		return 3;
	else 
		if (c == '*' || c == '+' || c == '?')
		return 4;
	else 
		return 10;
}

void Parser::makeAutomata() {
	makedAutomata = true;
	makePostfix();

	NFA nfa;
	nfa.build(postfix);
	nfa.print();

	dfa.build(nfa);
	dfa.print(false);
	dfa.minimize();
	dfa.print(true);
	//dfa.dot(cout);
}

void Parser::makePostfix() {
	
	postfix = toPostfix(reg, priority);
	
}


std::string Parser::toPostfix(std::string s, int(*priority)(char t)) {
	std::string answer = "";
	std::stack<char> st;

	for (auto &c : s) {
		if (c == '(')  {
			st.push(c);
		}
		else if (c == ')') {
			while (!st.empty() && st.top() != '(') {
				answer.push_back(st.top());
				st.pop();
			}
			if (!st.empty())
				st.pop();
			else
			{
				return answer;
			}
		}
		else {
			while (!st.empty()){
				char lst = st.top();
				if (priority(lst) >= priority(c)){
					answer.push_back(lst);
					st.pop();
				}
				else {
					break;
				}
			}
			st.push(c);
		}
	}
	while (!st.empty()) {
		answer.push_back(st.top());
		st.pop();
	}
	
	return answer;
}
