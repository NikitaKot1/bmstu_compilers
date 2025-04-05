#include <iostream> 
#include <string> 
#include <fstream> 

#include "Parser.hpp"

using namespace std; 

string to_norm(string s) {
	string temp = "";
	for (int i = 0; i < s.size() - 1; i++){
		char t1 = s[i];
		temp += t1;
		char t2 = s[i + 1];
		if (t1 != '(' && t1 != '|' && t2 != ')' && t2 != '|' && t2 != '.' && t2 != '*' && t2 != '+'){
			temp += '.';
		}
	}
	temp += s[s.size() - 1];
	return temp;

}
int main() {

	string s;
    cin >> s;
	s = to_norm(s);
	Parser matcher(s);
	matcher.makeAutomata();
	string test;
	cin >> test;

	if (matcher.match(test))
		cout << "YES" << endl;
	else 
		cout << "NO" << endl;	


	return 0;
}