#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"

using std::map;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

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
