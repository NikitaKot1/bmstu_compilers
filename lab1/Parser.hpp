#pragma once

#include <string>

#include "NFA.hpp"
#include "DFA.hpp"

class Parser
{
public:
	Parser(std::string s);
	~Parser();
	bool match(std::string s);
	void makeAutomata();
	std::string postfix;

protected:
	std::string reg;
	void makePostfix();
	bool makedAutomata;
	DFA dfa;

private: 
	std::string toPostfix(std::string s, int (*priority)(char t) );
};

