#include "DFA.hpp"
#include <algorithm>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;
DFA::DFA() : startState(0), existDevilState(false)
{
}


DFA::~DFA()
{
}

vector< vector<bool> > DFA::buildTable(int n, vector<bool> isTerminal, map<int, map<char, set<int> > > reverseTransition){
	queue <pair<int, int> > q;
	vector<vector<bool>> marked;
	marked.assign(n, vector<bool>());
	for (int i = 0; i < n; i++)
		marked[i].assign(n, false);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (!marked[i][j] && isTerminal[i] != isTerminal[j]){
		marked[i][j] = marked[j][i] = true;
		q.push(make_pair(i, j));
			}

	while (!q.empty()) {
		pair<int, int> temp = q.front();
		q.pop();
		for (char c: alphabet) {
			for (auto &r : reverseTransition[temp.first][c])
				for (auto &s : reverseTransition[temp.second][c])
					if (!marked[r][s]) {
				marked[r][s] = marked[s][r] = true;
				q.push(make_pair(r, s));
					}
		}
		 
	}
	

	return marked;
}

void DFA::buildDFA(vector<int> &component, vector<bool> isTerminal) {
	map<int, vector<int>> comp;
	map<int, int> stateToComp;

	for (int i = 0; i < component.size(); i++) 
		comp[component[i]].push_back(i);
	
	int n = component.size();
	vector <StateDFA> newStates(comp.size());
	vector <StateDFA> newTransition;
	
	for (auto &elem : comp) {
		if (elem.first == -1)
			continue;
		newStates[elem.first].states = elem.second;

		map<char, int> newTr;
		for (auto &e : elem.second) {
			if (isTerminal[e])
				newStates[elem.first].finalState = true;

			for (char c: alphabet) {
				if (transition[e].find(c) == transition[e].end())
					continue;
				else newTr[c] = component[transition[e][c]];
			}
		}

		newStates[elem.first].transition = newTr;	
	}
	
	states = newStates;
	transition.clear();
	buildTransition();

	startState = component[startState];
}

bool DFA::isExistDevilState() {
	return existDevilState;
}

void DFA::minimize() {
	int n = states.size() + 1;
	StateDFA devil;
	states.push_back(devil);

	for (int i = 0; i < states.size(); i++)
		for (char c: alphabet)
			if (states[i].transition.find(c) == states[i].transition.end())
				states[i].addLink(n - 1, c);
	existDevilState = true;

	vector <bool> isTerminal(n);
	for (int i = 0; i < n; i++)
		isTerminal[i] = states[i].isFinalState();
	vector <bool> StartAttainability(states.size());
	for (int i = 0; i < StartAttainability.size(); i++)
		StartAttainability[i] = false;

	dfs(0, StartAttainability);
	buildTransition();

	std::map<int, std::map<char, set<int>>> reverseTransition;
	for (auto &m : transition){
		for (auto &tr : m.second) {
			reverseTransition[tr.second][tr.first].insert(m.first);
		}
	}
	 
	vector<vector<bool>> distinctTable = buildTable(n, isTerminal, reverseTransition);

	vector<int> component;
	component.assign(n, -1);

	for (int i = 0; i < n; i++)
		if (!distinctTable[0][i])
			component[i] = 0;

	int componentsCount = 0;
	for (int i = 0; i < n; i++) {
		if (!StartAttainability[i])
			continue;
		if (component[i] == -1){
			componentsCount++;
			component[i] = componentsCount;

			for (int j = i + 1; j < n-1; j++)
				if (!distinctTable[i][j])
					component[j] = componentsCount;
		}
	}

	buildDFA(component, isTerminal);
}


void DFA::buildTransition() {
	for (int i = 0; i < states.size(); i++)
		transition[i] = states[i].transition;

}

void DFA::dfs(state start, vector<bool> &used) {
	used[start] = 1;
	for (char c: alphabet) {
		if (states[start].transition.find(c) == states[start].transition.end())
			continue;

		if (!used[states[start].transition[c]])
			dfs(states[start].transition[c], used);
	}
}

bool DFA::match(string s) {
	state _state = 0;
	int check = 0;
	int iter = 1;

	for (auto& c : s) {
		if (states[_state].transition.find(c) == states[_state].transition.end()) 
			return 0;
		
		if ((states[states[_state].transition[c]].isFinalState()))
			check = iter;

		_state = states[_state].transition[c];
		iter++;
	}

	return check == s.size();

}

bool DFA::StateDFA::isEqual(StateDFA &a){
	
	if (states.size() != a.states.size()) 
		return false; 
	
	for (size_t i = 0; i < states.size(); i++) {
		if (states[i] != a.states[i])
			return false;
	}
	return true;
}

DFA::StateDFA::StateDFA(vector<state> _states, state final) : finalState(false) {
	states = _states;
	sort(states.begin(), states.end());
	
	for (auto &n : states)
		if (n == final)
			finalState = true;
}

void DFA::build(NFA &a) {

	vector<state> first_node = {a.getStartState()};
	alphabet = a.getAlphabet();

	states.push_back(StateDFA(a.moveWithoutEpsilons(first_node), a.getFinalState()));

	stack<state> st;
	st.push(startState);

	while (!st.empty()) {

		state current = st.top();
		st.pop();
		
		for (char c: alphabet) {
			
			vector<int> temp_ids = a.move(states[current].states, c);
			StateDFA temp(temp_ids, a.getFinalState());

			if (temp_ids.size())
			{
				int pos = -1;
				for (size_t i = 0; i < states.size(); i++)
					if (states[i].isEqual(temp)) {
						pos = i;
					}
				if (pos == -1) {
					states.push_back(temp);
					st.push(states.size() - 1);
					pos = st.top();
				}
				states[current].addLink(pos, c);
			}
		}
	}	
	buildTransition();
}

void DFA::print(bool min) {
	std::ofstream f;
	if (min) f.open("../graph/MinFA.gv");
	else 	 f.open("../graph/DFA.gv");
	if (min) f << "digraph MinFA {\n";
	else 	 f << "digraph DFA {\n";

	cout << transition.size() << "\n";

	if (!isExistDevilState()) {
		for (int i = 0; i < states.size(); i++) {
			if (states[i].isFinalState())
			f << "  " << i << " -> "<<"end"<<"[label=\""<<"eps"<<"\"]\n";
		}
		f << "  " << "start" << " -> "<<startState<<"[label=\""<<"eps"<<"\"]\n";
		
		for (const auto& a : transition) {
			for (const auto& b : a.second) {
				f << "  " << a.first  << " -> "<<b.second<<"[label=\""<<char(b.first)<<"\"]\n";
			}
		}
	}
	else { 
		for (int i = 0; i < states.size() - 1; i++) {
			if (states[i].isFinalState())
			f << "  " << i << " -> "<<"end"<<"[label=\""<<"eps"<<"\"]\n";\
		}
		f << "  " << "start" << " -> "<<startState<<"[label=\""<<"eps"<<"\"]\n";
		for (const auto& a : transition) {
			for (const auto& b : a.second) {
				if (b.second != states.size() - 1)
					f << "  " << a.first  << " -> "<<b.second<<"[label=\""<<char(b.first)<<"\"]\n";
			}
		}
	
	}
	f << "}";
	f.close();
}