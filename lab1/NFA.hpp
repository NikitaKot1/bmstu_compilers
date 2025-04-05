#pragma once
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <fstream>

typedef std::pair<int, int> piece;

using state = int;

class NFA
{
public:
	NFA();
	~NFA();
	void build(std::string postfix);

	struct Link {
		state to;
		char c;
		Link(int _to, char _c) : to(_to), c(_c)
		{}
	};

	std::vector <state> move(std::vector <state> &fromStates, char c);
	std::vector <state> moveWithoutEpsilons(std::vector <state> &fromStates);
	state getStartState() {
		return startState;
	}
	state getFinalState() {
		return finalState;
	}
	std::set<char> getAlphabet() {
		return alphabet;
	}
	void print();

protected: 
	std::vector <state> justMove(std::vector <state> &fromStates, char c);
	std::stack<int> renumbering;
	std::set<char> alphabet;
	piece buildOr(piece a, piece b);
	piece buildConcat(piece a, piece b);
	piece buildSymbol(char c);
	piece buildKleeneclosure(piece a);
	piece buildPlus(piece a);
	state buildInit();
	piece buildEpsilon();

private:
	std::vector<std::vector <Link> > states;
	state startState;
	state finalState;
};

