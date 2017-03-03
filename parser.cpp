#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

#include "parser.h"

using std::map;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;
using std::stack;

// TODO function assumes proper input. 
Parser::Parser(string filename): epsilon("epsilon") { 

	// open the file. 
	std::ifstream cfg_file;

	
	// TODO exception? 
	cfg_file.open(filename);

	string full_line;
	// get the terminals.
	{
		int num_terminals;
		getline(cfg_file, full_line);
		std::stringstream ss(full_line);
	       	ss >> num_terminals;
		for(int i = 0; i < num_terminals; i++) {
			getline(cfg_file, full_line);
			terminals.insert(full_line);
		}	
	}

	// get the nonterminals. 
	{
		int num_non_terminals;
		getline(cfg_file, full_line);
		std::stringstream ss(full_line);
		ss >> num_non_terminals;
	
		for(int i = 0; i < num_non_terminals; i++) {
			getline(cfg_file, full_line);
			non_terminals.insert(full_line);
		}
	}

	// Should only ever be one start symbol, 
	// and this is where it will be, after nonterminals. 
	// get it now. 
	
	{
		getline(cfg_file, full_line);
		std::stringstream ss(full_line);
		ss >> start_symbol;
	}
	
	// get the number of rules. 
	{
		int num_rules;
		{
			getline(cfg_file, full_line);
			std::stringstream ss(full_line);
			ss >> num_rules;
		}
		
		for(int i=0;i<num_rules;i++) {
			string non_terminal;	
			string rule_token;	
			getline(cfg_file, full_line);
			std::stringstream ss(full_line);
			ss >> non_terminal;
			vector<string> production;
			while(ss >> rule_token) {
				production.push_back(rule_token);
			}
			std::pair<string, vector<string>> rule(non_terminal, production);
			rules.push_back(rule);	
		}
	}

	{
		set<string>::iterator i;
		for(i = non_terminals.begin(); i != non_terminals.end(); i++) {
			createFirstSets(*i);
		}
	}

	createLookupTable();
}

set<string> Parser::createFirstSets(string S) {
	// If we already know it, just return it. 
	map<string, set<string>>::iterator iter;
	iter = first_sets.find(S);
	if(iter != first_sets.end()) {
		return iter->second;
	}

	// This is going to be a nice recursive function. 
	set<string>::iterator it;
	// Case 1: S is a terminal. 
	// 	Set first_sets[S] = set(S)
	// 	return a reference to the created set. 	
	it = terminals.find(S);
	if(it != terminals.end()) {
	       	set<string> ret;
		ret.insert(S);
		first_sets[S] = ret;
		return ret;
	}
	
	// Case 2: S is a nonterminal. 
	// 	Get a production rule for the nonterminal S. 
	// 	Get the first set of the first token in the rule. 
	// 	Do the same for all production rules that expand S. 
	it = non_terminals.find(S);	
	std::vector<std::pair<std::string, std::vector<std::string>>>::iterator i;
	vector<string>::iterator j;
	set<string> ret;
	set<string> full_set;

	if(it != non_terminals.end()) {
		for(i = rules.begin(); i != rules.end(); i++) {
			if(i->first == S) {

				for(j = i->second.begin(); j != i->second.end(); j++) {

					ret = createFirstSets(*j);
					set<string>::iterator k;
					for(k = ret.begin(); k != ret.end(); k++) {
						full_set.insert(*k);
					}

					k = ret.find(epsilon);
					if(k == ret.end()) {
						break;
					}
				}
			}
		}
		first_sets[S] = full_set;
		return full_set;
	}

	// Dead zone, S is neither a terminal nor non terminal... 
	cout << "WTF?" << endl;
	set<string> huh;
	return huh;
}

// TODO make this function a bit better, 
// ANd have it out put a tree. 
void Parser::parseInput(vector<string> sentence) {

	// put all of the input into a stack. 
	vector<string>::reverse_iterator it;
	stack<string> input_stack;
	for(it = sentence.rbegin(); it != sentence.rend(); it++) {
		input_stack.push(*it);
	}

	// Now create a stack for our parse symbol logic. 
	stack<string> parse_stack;
	parse_stack.push(start_symbol);

	// the symbol to match in our table. 
	string next_input;

	// iterator to rule that we need. 
	vector<pair<string, vector<string>>>::iterator ri;
	std::map<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::vector<std::string>>>::iterator>::iterator exists;
	while(!input_stack.empty()) {

		// Check if the top of the parse stack is a terminal
		// or nonterminal. If it's a terminal, then we need
		// to be able to match it, and pop it off. If it is
		// a terminal, but doesn't match, we reject input. 
		set<string>::iterator si;

		if(parse_stack.empty()) {
			cout << "Does not match." << endl;
			return;
		}

		si = terminals.find(parse_stack.top());

		if(si != terminals.end()) {
			if(*si == input_stack.top()) {
				input_stack.pop();
				parse_stack.pop();
			} else {
				cout << "No match." << endl;
				return;
			}
		} else {
			// must be a non terminal. 
			pair<string, string> key(parse_stack.top(), input_stack.top());
			exists = rule_lookup.find(key);

			if(exists == rule_lookup.end()) {
				cout << "No match in lookup table." << endl;
				return;
			}
			
			ri = rule_lookup[key];
			parse_stack.pop();

			vector<string>::reverse_iterator vsi;
			for(vsi = ri->second.rbegin(); vsi != ri->second.rend(); vsi++) {
				parse_stack.push(*vsi);
			}
		}
	}

	if(parse_stack.empty()) {
		cout << "Accepted" << endl;
	} else {
		cout << "Rejected" << endl;
	}
}	







// TODO
// Add in a check to make sure that
// when we insert into the rule lookup, 
// there isn't already something there. 
// (Because then grammar isn't LL(1))
void Parser::createLookupTable() {
	// Go through the list of rules. 
	// For each rule R: 
	// 	load the production P. 
	// 	Take the "first" of the first symbol in P. 
	// 	For every symbol S in the set, 
	// 		map(non_terminal, s) = R. 

	// (rit == rules iterator) 	
	std::vector<std::pair<std::string, std::vector<std::string>>>::iterator rit;
	std::vector<string>::iterator first_sym;

	// first set of the first symbol in the production. 
	std::set<string> f_s;
	std::set<string>::iterator si;
	
	for(rit = rules.begin(); rit != rules.end(); rit++) {
		first_sym = (rit->second).begin();
		f_s = first_sets[*first_sym];

		for(si = f_s.begin(); si != f_s.end(); si++) {
			std::pair<string, string> key(rit->first, *si);
			rule_lookup[key] = rit;
		}
	}
}

void Parser::printLookupTable() {

	map<pair<string, string>, vector<pair<string, vector<string>>>::iterator>::iterator it;
	
	for(it = rule_lookup.begin(); it != rule_lookup.end(); it++) {
		cout << "(" << (it->first).first << ", " << (it->first).second << ") = ";
		printRule(it->second);
	}
}

void Parser::printRule(vector<pair<string, vector<string>>>::iterator i) {

	vector<string>::iterator k;
	cout << i->first << " -> ";

	for(k = i->second.begin(); k != i->second.end(); k++) {
		cout << *k << " ";
	}

	cout << endl;
}


void Parser::printRules() {

	std::vector<pair<string, vector<string>>>::iterator rules_it;
	std::vector<string>::iterator it;

	for(rules_it = rules.begin(); rules_it != rules.end(); rules_it++) {
		cout << rules_it->first << ": ";
		for(it = rules_it->second.begin(); it != rules_it->second.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

void Parser::printFirstSets() {

	map<string, set<string>>::iterator i;
	for(i = first_sets.begin(); i != first_sets.end(); i++) {
		cout << i->first << " -> { ";
		set<string>::iterator j;
		for(j = i->second.begin(); j != i->second.end(); j++) {
			cout << *j << " ";
		}
		cout << "}" <<  endl;
	}
}
