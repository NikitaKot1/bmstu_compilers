#include "NFA.hpp"
const char eps = 0;
#include <stack>
#include <map>
#include <iostream>


NFA::NFA()
{
}


NFA::~NFA()
{
}



int NFA::buildInit() {
    int newNumber = 0;
	if (renumbering.empty()){
		states.push_back(std::vector<Link>());
		newNumber = states.size() - 1;
	}
	else {
		newNumber = renumbering.top();
		renumbering.pop();
	}

	return newNumber;
}

piece NFA::buildEpsilon() {

	int start = buildInit(), end = buildInit();
	states[start].push_back(Link(end, eps));

	return piece(start, end);
}

piece NFA::buildSymbol(char c) {
	
    int start = buildInit(), end = buildInit();
    states[start].push_back(Link(end, c));
	alphabet.insert(c);

    return piece(start, end);
}

piece NFA::buildKleeneclosure(piece a) {
    int start = buildInit(), end = buildInit();
	states[start].push_back(Link(a.first, eps));
	states[start].push_back(Link(end, eps));
	states[a.second].push_back(Link(a.first, eps));
	states[a.second].push_back(Link(end, eps));

	return piece(start, end);
}

piece NFA::buildPlus(piece a) {

    int start = buildInit(), end = buildInit();
	states[start].push_back(Link(a.first, eps));
	states[a.second].push_back(Link(a.first, eps));
	states[a.second].push_back(Link(end, eps));

	return piece(start, end);
}

piece NFA::buildOr(piece a, piece b) {
    
	int start = buildInit(), end = buildInit();
	states[start].push_back(Link(a.first, eps));
	states[start].push_back(Link(b.first, eps));
	states[b.second].push_back(Link(end, eps));
	states[a.second].push_back(Link(end, eps));

	return piece(start, end);
}

piece NFA::buildConcat(piece a, piece b) {
	unsigned int n2_s = a.first, n2_e = a.second;
	unsigned int n1_s = b.first, n1_e = b.second;
	
	for (auto& link : states[b.first]){
		states[a.second].push_back(link);
	}
	states[b.first].clear();
	renumbering.push(b.first);

	return piece(a.first, b.second);
}
piece pop(std::stack<piece> &s){
	piece temp = s.top();
	s.pop();
	return temp;
}
void NFA::build(std::string postfix) {
	startState = buildInit();

	std::stack<piece> st;
	for (auto& c : postfix){		
	    if (c == '*') {
			st.push(buildKleeneclosure(pop(st)));
		}
		else if (c == '+') {
			st.push(buildPlus(pop(st)));
		}
		else if (c == '.') {
			st.push(buildConcat(pop(st), pop(st)));
		}
		else if (c == '|') {
			st.push(buildOr(pop(st), pop(st)));
		}
		else if (c == '@') {
			st.push(buildEpsilon());
		}
		else {
			st.push(buildSymbol(c));
		}
	}
	if (st.empty()){
		return;
	}
	while (!st.empty()){
		startState = st.top().first;
		finalState = st.top().second;
		st.pop();
	}
}

std::vector<state> NFA::justMove(std::vector <state> &fromStates, char c) {
	std::vector<state> ans;
	
	for (auto& n : fromStates)
		for (auto& link : states[n])
			if (link.c == c) {
				ans.push_back(link.to);
			}

	return ans;
}

std::vector <state> NFA::moveWithoutEpsilons(std::vector <state> &fromStates) {
	std::vector<state> ans;
	std::stack<state> st;
	std::map<state, bool> cache;

	for (auto& n : fromStates) {
		ans.push_back(n);
		st.push(n);
		cache[n] = true;
	}

	while (!st.empty()) {
		state cur = st.top();
		st.pop();
		cache[cur] = true;
		for (auto& n : states[cur]){
			if (!n.c && cache.find(n.to) == cache.end()){
				st.push(n.to);
				ans.push_back(n.to);
			}
		}
	}

	return ans;
}

std::vector <state> NFA::move(std::vector <state> &fromStates, char c) {

	std::vector<state> res = justMove(fromStates, c);
	return moveWithoutEpsilons(res);

}

void NFA::print() {
	std::ofstream f("../graph/NFA.gv");
    f << "digraph NFA {\n";
	for (int i = 0; i < states.size(); i++) {
		for (auto ln: states[i]) {
			if (ln.c)
				f << "  " << i << " -> "<<ln.to<<"[label=\""<<char(ln.c)<<"\"]\n";
			else
				f << "  " << i << " -> "<<ln.to<<"[label=\""<<"eps"<<"\"]\n";
		}
	}
	f << "  " << "start" << " -> "<<startState<<"[label=\""<<"eps"<<"\"]\n";
	f << "  " << finalState << " -> "<<"end"<<"[label=\""<<"eps"<<"\"]\n";
	f << "}";
	f.close();
}