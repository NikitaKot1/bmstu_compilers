#include "fincs.hpp"
#include "grammar.hpp"

#include <iostream>

void remove_left_recursion_nonterms(
                map<string, vector<vector<string>>> &rules, 
                map<string, vector<vector<string>>> &new_rules,
                vector<string> &new_nonterminal) {
    new_rules = rules;
    string epsilon = "eps";
    
    for (auto rule: rules) {
        int add_ind = 1;
        vector<vector<string>> alpha;
        vector<vector<string>> beta;
        for (int i = 0; i < rule.second.size(); i++) {
            if (rule.second[i][0] == rule.first && rule.second[i].size() >= 1) {
                vector<string> a;
                a.insert(a.end(), 
                    rule.second[i].begin()+1,
                    rule.second[i].end());
                alpha.push_back(a);
            }
            else {
                vector<string> b;
                b.insert(b.end(), 
                    rule.second[i].begin(),
                    rule.second[i].end());
                beta.push_back(b);
            }
        }
        if (beta.size() == 0) {
            vector<string> b;
            beta.push_back(b);
        }
        if (!alpha.empty()) {
            string new_nont = rule.first + to_string(add_ind);
            new_nonterminal.push_back(new_nont);
            new_rules.insert({rule.first, {}});
            new_rules.insert({new_nont, {}});
            for (int i = 0; i < beta.size(); i++) {
                vector<string> symb = beta[i];
                if (beta[i][0] == epsilon) {
                    symb.clear();
                }
                symb.push_back(new_nont);
                new_rules[rule.first].push_back(symb);
            }
            for (int i = 0; i < alpha.size(); i++) {
                vector<string> symb = alpha[i];
                symb.push_back(new_nont);
                new_rules[new_nont].push_back(symb);
            }
            new_rules[new_nont].push_back({epsilon});
        }
    }
}

grammar remove_left_recursion_indirect(grammar g) {
    auto rules = g.rules;
    vector<string> nonterminal;
    nonterminal.reserve(g.rules.size());
    transform(g.rules.begin(), g.rules.end(),
                back_inserter(nonterminal),
            [](const auto &pai) {return pai.first;});
    
    auto new_nonterminal_arr = g.nonterminal;

    for (int i = 0; i < nonterminal.size(); i++) {
        string Ai = nonterminal[i];
        for (int j = 0; j < i; j++) {
            auto ai_prod_arr = rules[Ai];
            string Aj = nonterminal[j];
            auto aj_prod_arr= rules[Aj];

            vector<vector<string>> new_prod_arr;

            for (int k = 0; k < ai_prod_arr.size(); k++) {
                vector<string> new_prod;
                if (ai_prod_arr[k][0] == Aj) {
                    for (auto aj_prod: aj_prod_arr) {
                        if (aj_prod[aj_prod.size()-1] == "eps") {
                            reverse(aj_prod.begin(), aj_prod.end());
                        }
                        vector<string> a;
                        a.insert(a.end(), aj_prod.begin(), aj_prod.end());
                        a.insert(a.end(), ai_prod_arr[k].begin()+1, ai_prod_arr[k].end());
                        new_prod_arr.push_back(a);
                    }
                }
                else {
                    new_prod.insert(new_prod.end(),
                        ai_prod_arr[k].begin(), ai_prod_arr[k].end());
                }
                if (!new_prod.empty())
                    new_prod_arr.push_back(new_prod);
            }
            rules[Ai] = new_prod_arr;
        }
        map<string, vector<vector<string>>> new_rules;
        vector<string> new_nonterminal;
        map<string, vector<vector<string>>> rules_slash;
        rules_slash[Ai] = rules[Ai];
        remove_left_recursion_nonterms(
            rules_slash,
            new_rules,
            new_nonterminal
        );
        new_nonterminal_arr.insert(new_nonterminal_arr.end(),
                new_nonterminal.begin(), new_nonterminal.end());
        for (auto r: new_rules)
            rules[r.first] = new_rules[r.first];
    }
    g.rules = rules;
    g.nonterminal = new_nonterminal_arr;
    return g;
}


grammar remove_unattainable_symbols(grammar g) {
 
    set<string> nonterminal;
    transform(g.nonterminal.begin(), g.nonterminal.end(),
                inserter(nonterminal, nonterminal.end()),
            [](const auto &pai) {return pai;});
    set<string> pred;
    set<string> next{g.startsymbol};

    while (next != pred) {
        auto c = pred;
        pred = next;
        next = c;

        for (auto &rule: g.rules) {
            if (pred.find(rule.first) != pred.end()) {
                for (auto &alter: rule.second) {
                    for (int i = 0; i < alter.size(); i++) {
                        if (nonterminal.find(alter[i]) != nonterminal.end())
                            next.insert(alter[i]);
                    }
                }
            }
        }
        set_union(next.begin(), next.end(), pred.begin(), pred.end(), 
            inserter(next, next.end()));
    }
       
    grammar new_g;
    set_intersection(nonterminal.begin(), nonterminal.end(),
                    next.begin(), next.end(), back_inserter(new_g.nonterminal));
    new_g.startsymbol = g.startsymbol;

    set<string> terminal;
    map<string, vector<vector<string>>> rules;
    for (auto &rule: g.rules) {
        if (next.find(rule.first) != next.end()) {
            rules[rule.first] = rule.second;
            for (auto &alter: rule.second) {
                for (int i = 0; i < alter.size(); i++) {
                    if (nonterminal.find(alter[i]) == nonterminal.end() && alter[i] != "eps")
                        terminal.insert(alter[i]);
                }
            }
        }
    }

    transform(terminal.begin(), terminal.end(),
                back_inserter(new_g.terminal),
            [](const auto &pai) {return pai;});
    new_g.rules = rules;
    return new_g;
}


grammar remove_left_recursion_immediate(grammar g) {
    auto new_rules = g.rules;
    set<string> new_nonterminal;
    transform(g.nonterminal.begin(), g.nonterminal.end(),
                inserter(new_nonterminal, new_nonterminal.end()),
            [](const auto &pai) {return pai;});
    for (auto &rule: g.rules) {
        int add_ind = 1;
        vector<vector<string>> alpha;
        vector<vector<string>> beta;
        for (int i = 0; i < rule.second.size(); i++) {
            if (rule.second[i][0] == rule.first && rule.second[i].size() >= 1) {
                vector<string> a;
                a.insert(a.end(), 
                    rule.second[i].begin()+1,
                    rule.second[i].end());
                alpha.push_back(a);
            }
            else {
                vector<string> b;
                b.insert(b.end(), 
                    rule.second[i].begin(),
                    rule.second[i].end());
                beta.push_back(b);
            }
        }

        if (!alpha.empty()) {
            string new_nont = rule.first + to_string(add_ind);
            new_nonterminal.insert(new_nont);
            new_rules.insert({rule.first, {}});
            new_rules.insert({new_nont, {}});
            if (beta.empty())
                new_rules[rule.first] = {{new_nont}};
            
            for (int i = 0; i < beta.size(); i++) {
                if (beta[i][beta[i].size()-1] == "eps") {
                    new_rules[rule.first].push_back({new_nont});
                    new_rules[rule.first].push_back(beta[i]);
                }
                else {
                    new_rules[rule.first].push_back(beta[i]);
                    vector<string> symb = beta[i];
                    symb.push_back(new_nont);
                    new_rules[rule.first].push_back(symb);
                }
            }
            for (int i = 0; i < alpha.size(); i++) {
                new_rules[rule.first].push_back(alpha[i]);
                vector<string> symb = alpha[i];
                symb.push_back(new_nont);
                new_rules[rule.first].push_back(symb);

            }
        }
    }
    g.rules = new_rules;
    transform(new_nonterminal.begin(), new_nonterminal.end(),
            back_inserter(g.nonterminal),
            [](const auto &pai) {return pai;});
    return g;
}

vector<string> get_longest_prefix(vector<vector<string>> rule) {
    vector<string> prod_str_arr;
    for (auto &alter: rule) {
        string s;
        for (int i = 0; i < alter.size(); i++)
            s += alter[i] + "|";
        prod_str_arr.push_back(s);
    }
    sort(prod_str_arr.begin(), prod_str_arr.end());
    string max_pref;
    for (int i = 0; i < prod_str_arr.size()-1; i++) {
        string p1 = prod_str_arr[i];
        string p2 = prod_str_arr[i+1];
        string pref;
        int len = p1.size() > p2.size() ? p2.size() : p1.size();
        for (int j = 0; j < len; j++) {
            if (p1[j]==p2[j])
                pref += p1[j];
            else {
                if (pref.size() > max_pref.size()) 
                    max_pref = pref;
                pref.clear();
                break;
            }
        }
        if (pref.size() > max_pref.size()) 
            max_pref = pref;
    }
    vector<string> max_pref_arr;
    istringstream ss(max_pref);
    string tok;
    while (getline(ss, tok, '|')) 
        max_pref_arr.push_back(tok);
    return max_pref_arr;
}

grammar left_factorisation(grammar g) {
    map<string, vector<vector<string>>> rules;
    map<string, bool> eps_dist;

    for (auto &rule: g.rules) {
        cout << rule.first << " " << rule.second.size() << "\n";
        eps_dist[rule.first] = false;
        vector<string> prefix = get_longest_prefix(rule.second);
        if (prefix.empty())
            continue;
        cout << "a\n";
        string new_symb = rule.first;
        while (!prefix.empty()) {
            new_symb += '1';

            vector<vector<string>> beta_arr;
            vector<vector<string>> gamma_arr;

            for (auto &alter: rule.second) {
                if (equal(prefix.begin(), prefix.end(), alter.begin())) {
                    vector<string> beta;
                    for (int i = prefix.size(); i < alter.size(); i++)
                        beta.push_back(alter[i]);
                    if (!beta.empty())
                        beta_arr.push_back(beta);
                    else
                        eps_dist[new_symb] = true;
                }
                else
                    gamma_arr.push_back(alter);
            }

            cout << "a\n";
            g.nonterminal.push_back(new_symb);
            vector<string> new_rule;
            new_rule.insert(new_rule.end(), prefix.begin(), prefix.end());
            new_rule.push_back(new_symb);
            vector<vector<string>> nn_rule;
            nn_rule.push_back(new_rule);
            nn_rule.insert(nn_rule.end(), gamma_arr.begin(), gamma_arr.end());

            rules[rule.first] = nn_rule;
            rules[new_symb] = beta_arr;

            rule.second = rules[rule.first];
            prefix = get_longest_prefix(rule.second);

            cout << "a: " << prefix.size() << "\n";
        }
    }
    cout << "a\n";
    for (auto &rule: rules) {
        g.rules[rule.first] = rules[rule.first];
        if (eps_dist.find(rule.first) != eps_dist.end() && eps_dist[rule.first])
            g.rules[rule.first].push_back({"eps"});
    }
    return g;
}