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

	nfa.add_state("start");
	nfa.add_state("alpha");
	nfa.add_state("beta");
	nfa.add_state("gamma");

	nfa.set_start(0);
	nfa.set_final(1);

	nfa.add_transition(0, 'a', 1);
	nfa.add_transition(1, 'b', 1);
	nfa.add_transition(1, 'a', 2);
	nfa.add_transition(2, 'a', 2);
	nfa.add_transition(2, 'b' ,1);

	nfa.add_epsilon_transition(1, 2);
	nfa.add_epsilon_transition(1, 3);
	nfa.add_epsilon_transition(2, 0);

	nfa.compute_epsilon_closures();
	nfa.print_info();

	//dfa.run();

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
