#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "parsetree.h"


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {

	(void)argc;
	(void)argv;

	/* equal, period, comma, lparen, rparen */
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

	

	
	string cfg_file("../cfg_files/test010.cfg");
	Parser p(cfg_file);

	Lexer l(cin, rgx_pattern, symbols);
	while(true) {
		vector<Token> res = l.getSentence();
		//print_token_vector(res);
		ParseTreeNode *root = p.parseTokens(res);
		root->print(0);
	}
	
	return 0;
}
