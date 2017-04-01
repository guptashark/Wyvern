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
		nfa.set_final("id-lead", "C_ID");
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

	// we add support for newlines, (important for interpreter behavior)
	// (splitting apart newlines and tabs and spaces. 
	// important for pythonesque syntax
	{
		nfa.add_state("newline-begin");
		nfa.add_epsilon_transition("start", "newline-begin");
		nfa.add_state("newline-end");
		nfa.add_transition("newline-begin", "\n", "newline-end");
		nfa.set_final("newline-end", "NEWLINE");
	}

	// support for tabs
	{
		nfa.add_state("tab-begin");
		nfa.add_epsilon_transition("start", "tab-begin");
		nfa.add_state("tab-end");
		nfa.add_transition("tab-begin", "\t", "tab-end");
		nfa.set_final("tab-end", "TAB");
	}

	// support for spaces
	{
		nfa.add_state("space-begin");
		nfa.add_epsilon_transition("start", "space-begin");
		nfa.add_state("space-end");
		nfa.add_transition("space-begin", ' ', "space-end");
		nfa.set_final("space-end", "SPACE");
	}

	// add support for lbrace.
	{
		nfa.add_state("lbrace-begin");
		nfa.add_epsilon_transition("start", "lbrace-begin");
		nfa.add_state("lbrace-end");
		nfa.add_transition("lbrace-begin", "{", "lbrace-end");
		nfa.set_final("lbrace-end", "LBRACE");
	}

	// add support for rbrace, 
	{
		nfa.add_state("rbrace-begin");
		nfa.add_epsilon_transition("start", "rbrace-begin");
		nfa.add_state("rbrace-end");
		nfa.add_transition("rbrace-begin", "}", "rbrace-end");
		nfa.set_final("rbrace-end", "RBRACE");
	}

	{
		nfa.add_state("lparen-begin");
		nfa.add_epsilon_transition("start", "lparen-begin");
		nfa.add_state("lparen-end");
		nfa.add_transition("lparen-begin", "(", "lparen-end");
		nfa.set_final("lparen-end", "LPAREN");		
	}

	// add support for lparen
	{
		nfa.add_state("rparen-begin");
		nfa.add_epsilon_transition("start", "rparen-begin");
		nfa.add_state("rparen-end");
		nfa.add_transition("rparen-begin", ")", "rparen-end");
		nfa.set_final("rparen-end", "RPAREN");		
	}

	// add support for period
	{
		nfa.add_state("period-begin");
		nfa.add_epsilon_transition("start", "period-begin");
		nfa.add_state("period-end");
		nfa.add_transition("period-begin", ".", "period-end");
		nfa.set_final("period-end", "PERIOD");
	}

	// add support for comma
	{	
		nfa.add_state("comma-begin");
		nfa.add_epsilon_transition("start", "comma-begin");
		nfa.add_state("comma-end");
		nfa.add_transition("comma-begin", ",", "comma-end");
		nfa.set_final("comma-end", "COMMA");
	}

	// add support for ampersand
	{
		nfa.add_state("amp-begin");
		nfa.add_epsilon_transition("start", "amp-begin");
		nfa.add_state("amp-end");
		nfa.add_transition("amp-begin", "&", "amp-end");
		nfa.set_final("amp-end", "AMPERSAND");
	}

	// add support for lsqbrace ([)
	{
		nfa.add_state("lsqbrace-begin");
		nfa.add_epsilon_transition("start", "lsqbrace-begin");
		nfa.add_state("lsqbrace-end");
		nfa.add_transition("lsqbrace-begin", "[", "lsqbrace-end");
		nfa.set_final("lsqbrace-end", "LSQBRACE");
	}

	// add support for rsqbrace (])	
	{
		nfa.add_state("rsqbrace-begin");
		nfa.add_epsilon_transition("start", "lsqbrace-begin");
		nfa.add_state("rsqbrace-end");
		nfa.add_transition("rsqbrace-begin", "]", "rsqbrace-end");
		nfa.set_final("rsqbrace-end", "RSQBRACE");
	}

	// add support for equal symbol

	{
		nfa.add_state("eq-begin");
		nfa.add_epsilon_transition("start", "eq-begin");
		nfa.add_state("eq-end");
		nfa.add_transition("eq-begin", "=", "eq-end");
		nfa.set_final("eq-end", "EQUAL");
	}


	// try adding in support for relational operator ==
	{
		nfa.add_state("relop-eq-begin");
		nfa.add_state("relop-eq-mid");
		nfa.add_state("relop-eq-end");
		nfa.add_epsilon_transition("start", "relop-eq-begin");
		nfa.add_transition("relop-eq-begin", "=", "relop-eq-mid");
		nfa.add_transition("relop-eq-mid", "=", "relop-eq-end");
		nfa.set_final("relop-eq-end", "RELOP_EQ");
	}

	// DOn't commit until we have the following done: 
	// TODO			

	// Recognize < and <=
	{
		nfa.add_state("relop-lt-begin");
		nfa.add_state("relop-lt-end");
		nfa.add_state("relop-lteq-end");
		nfa.add_epsilon_transition("start", "relop-lt-begin");
		nfa.add_transition("relop-lt-begin", "<", "relop-lt-end");
		nfa.add_transition("relop-lt-end", "=", "relop-lteq-end");
		nfa.set_final("relop-lt-end", "LT");
		nfa.set_final("relop-lteq-end", "LTEQ");

	}	

	// recognize > and >=
	{
		nfa.add_state("relop-gt-begin");
		nfa.add_state("relop-gt-end");
		nfa.add_state("relop-gteq-end");
		nfa.add_epsilon_transition("start", "relop-gt-begin");
		nfa.add_transition("relop-gt-begin", ">", "relop-gt-end");
		nfa.add_transition("relop-gt-end", "=", "relop-gteq-end");
		nfa.set_final("relop-gt-end", "GT");
		nfa.set_final("relop-gteq-end", "GTEQ");
	}

	// recognize semicolon
	{
		nfa.add_state("semicolon-begin");
		nfa.add_state("semicolon-end");
		nfa.add_epsilon_transition("start", "semicolon-begin");
		nfa.add_transition("semicolon-begin", ";", "semicolon-end");
		nfa.set_final("semicolon-end", "SEMICOLON");
	}

	// recognize colon
	{
		nfa.add_state("colon-begin");
		nfa.add_state("colon-end");
		nfa.add_epsilon_transition("start", "colon-begin");
		nfa.add_transition("colon-begin", ":", "colon-end");
		nfa.set_final("colon-end", "COLON");
	}

	// recognize strings with escaped quotes
	// just build the string of possible chars we can have in a literal
	string all_printable;
	for(char c = 32; c < 126; c++) {
		if(c != '"' && c != '\\') all_printable.push_back(c);
		
	}

	{
		nfa.add_state("strlit-begin");
		nfa.add_state("strlit-lead-dq");
		nfa.add_state("strlit-mid");
		nfa.add_state("strlit-end");
		nfa.add_state("strlit-esc");
		nfa.add_state("strlit-esc-dq");
		
		nfa.add_epsilon_transition("start", "strlit-begin");
		nfa.add_transition("strlit-begin", "\"", "strlit-mid");
		nfa.add_transition("strlit-mid", all_printable, "strlit-mid");
		nfa.add_transition("strlit-mid", "\"", "strlit-end");
		nfa.set_final("strlit-end", "STRING_LITERAL");
		
	}


	// This part clearly doesn't work. 
	// recognize EOF?? 
	{
		nfa.add_state("EOF-begin");
		nfa.add_state("EOF-end");
		nfa.add_epsilon_transition("start", "EOF-begin");
		nfa.add_transition("EOF-begin", '\0' ,"EOF-end");
		nfa.set_final("EOF-end", "EOF");
	}
	
	
}

int main(int argc, char *argv[]) {

	SourceReader sr("../cfg_files/c_identifiers");
	//SourceReader sr;
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

	string result;
	while(result != "EOF") {
		result = nfa.run();
	}
	
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
