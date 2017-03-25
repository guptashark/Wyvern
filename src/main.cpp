#include <iostream>
#include <stack>
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "parsetree.h"
#include "deterministicfa.h"
#include "nondeterministicfa.h"

using namespace std;

int main(int argc, char *argv[]) {

	NonDeterministicFA nfa;

	nfa.add_state("start"); // 0
	nfa.add_state("for-start");
	nfa.add_state("for-f");
	nfa.add_state("for-fo");
	nfa.add_state("for-for");

	nfa.add_state("foal-start"); // 5
	nfa.add_state("foal-f");
	nfa.add_state("foal-fo");
	nfa.add_state("foal-foa");
	nfa.add_state("foal-foal");

	nfa.add_epsilon_transition(0, 1);
	nfa.add_transition(1, 'f', 2);
	nfa.add_transition(2, 'o', 3);
	nfa.add_transition(3, 'r', 4);

	nfa.add_epsilon_transition(0, 5);
	nfa.add_transition(5, 'f', 6);
	nfa.add_transition(6, 'o', 7);
	nfa.add_transition(7, 'a', 8);
	nfa.add_transition(8, 'l', 9);

	nfa.set_start(0);

	nfa.compute_epsilon_closures();
	nfa.print_info();
	nfa.run();
	return 0;
}


/*
	(void)argc;
	(void)argv;

	// equal, period, comma, lparen, rparen 
	string single_symbols ( "(=)|(\\.)|(\\,)|(\\()|(\\))");
	string identifier ("([_[:alnum:]]+)|");
	string whitespace ("([[:space:]]+)|");

	
	string rgx_pattern = identifier + whitespace + single_symbols;

	string array_of_symbols[8] = {
		"IDENTIFIER",
		"SPACE",
		"EQUAL",
		"PERIOD",
		"COMMA",
		"LPAREN",
		"RPAREN"
	};

	vector<string> symbols;
	for(int i =0; i < 7; i++) {
		symbols.push_back(array_of_symbols[i]);
	}

	

	
	string cfg_file("../cfg_files/wyvern.cfg");
	Parser p(cfg_file);

	Lexer l(cin, rgx_pattern, symbols);
	while(true) {
		vector<Token> res = l.getSentence();
		//print_token_vector(res);
		ParseTreeNode *root = p.parseTokens(res);
		if(root != NULL) root->print(0);
		delete root;
	}
	
	return 0;
*/
