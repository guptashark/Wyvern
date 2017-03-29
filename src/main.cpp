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
#include "source_reader.h"

using namespace std;

int main(int argc, char *argv[]) {

	SourceReader sr("../cfg_files/test3.wvn");
	list<string> los;
	los.push_back("for");
	los.push_back("else");
	los.push_back("int");
	los.push_back("char");
	los.push_back("using");
	los.push_back("return");

	NonDeterministicFA nfa(los);
	// add in basic identifiers
	nfa.add_state("identifier-begin");
	nfa.add_epsilon_transition("start", "identifier-begin");
	nfa.add_state("identifier-end");

	string lower = "abcdefghijklmnopqrstuvwxyz";

	nfa.add_transition("identifier-begin", lower, "identifier-end");
	nfa.add_transition("identifier-end", lower, "identifier-end");
	nfa.set_final("identifier-end");

	// need this as a delimiter
	nfa.add_state("ws-begin");
	nfa.add_state("ws-end");
	nfa.add_epsilon_transition("start", "ws-begin");
	nfa.add_transition("ws-begin", '\n', "ws-end");
	nfa.set_final("ws-end");

	nfa.set_source_reader(&sr);

	nfa.compute_epsilon_closures();
//	nfa.print_info();

	nfa.run();
	nfa.run();
	nfa.run();
	nfa.run();
	nfa.run();
}
//	nfa.convert_to_dfa();

/*
	return 0;
}
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
