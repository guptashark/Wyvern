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
#include "hardcodenfa.h"
#include "source_reader.h"

using namespace std;

void hardcoded_C_lexer(HardCodeNFA &nfa) {
 	string uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string lowercase("abcdefghijklmnopqrstuvwxyz");
	string nonzero_digit("123456789");
	string digit = nonzero_digit + "0";
 	string alphanum = digit + uppercase + lowercase;

	
	// assume the nfa is empty, no states, nothing. 

	nfa.add_state("start");
	nfa.set_start(0);
	// our identifiers can't start with an underscore, 
	// alphanumeric characters are fine. 
	{
		nfa.add_state("id-begin");
		nfa.add_epsilon_transition("start", "id-begin");
		nfa.add_state("id-lead");
		nfa.add_transition("id-begin", uppercase + lowercase, "id-lead");
		nfa.add_state("id-mid");
		nfa.add_transition("id-lead", alphanum + "_", "id-mid");
		nfa.add_transition("id-mid", alphanum + "_", "id-mid");
		nfa.add_state("id-end");
		nfa.add_transition("id-mid", alphanum, "id-end");
		nfa.set_final("id-end", "C_ID");
	}

	// we add in support for positive and negative integers
	{
		nfa.add_state("int-begin");
		nfa.add_epsilon_transition("start", "int-begin");
		nfa.add_state("int-lead");
		nfa.add_state("int-neg_symbol");
		nfa.add_transition("int-begin", "-", "int-neg_symbol");
		nfa.add_transition("int-neg_symbol", nonzero_digit, "int-lead");
		nfa.add_transition("int-begin", nonzero_digit, "int-lead");
		nfa.add_transition("int-lead", digit, "int-lead");
		nfa.set_final("int-lead", "C_INT");
	}

	// we add support for whitespace so that this works on a simple test file. 
	{
		nfa.add_state("ws-begin");
		nfa.add_epsilon_transition("start", "ws-begin");
		nfa.add_state("ws-end");
		nfa.add_transition("ws-begin", " \n\t", "ws-end");
		nfa.add_transition("ws-end", " \n\t", "ws-end");
		nfa.set_final("ws-end", "WS");
	}
}

int main(int argc, char *argv[]) {

	SourceReader sr("../cfg_files/c_identifiers");
	/*
	list<string> los;
	los.push_back("for");
	los.push_back("else");
	los.push_back("int");
	los.push_back("char");
	los.push_back("using");
	los.push_back("return");

	HardCodeNFA nfa(los);
	// add in basic identifiers
	nfa.add_state("identifier-begin");
	nfa.add_epsilon_transition("start", "identifier-begin");
	nfa.add_state("identifier-end");

	string lower = "abcdefghijklmnopqrstuvwxyz";

	nfa.add_transition("identifier-begin", lower, "identifier-end");
	nfa.add_transition("identifier-end", lower, "identifier-end");
	nfa.set_final("identifier-end", "identifier");


	// need this as a delimiter
	nfa.add_state("ws-begin");
	nfa.add_state("ws-end");
	nfa.add_epsilon_transition("start", "ws-begin");
	nfa.add_transition("ws-begin", '\n', "ws-end");
	nfa.set_final("ws-end", "whitespace");

	nfa.set_source_reader(&sr);

	nfa.compute_epsilon_closures();
//	nfa.print_info();

	nfa.run();
	
//	nfa.convert_to_dfa();
 */

	
	HardCodeNFA nfa;
	hardcoded_C_lexer(nfa);
	nfa.set_source_reader(&sr);	

	nfa.run();
	nfa.run();
	nfa.run();
	nfa.run();
	nfa.run();
	nfa.run();
	
	return 0;
}


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
