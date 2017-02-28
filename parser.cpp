#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"

using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

// TODO function assumes proper input. 
Parser::Parser(string filename) { 

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
};

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
