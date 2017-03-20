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


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

// We're going to use ALSU Dragon book
// To try and implement a better lexer
// We'll call it: MyLexer, 
// and then write all the functions for it
// and then later put it into a file of its own. 
// But for now, comment out all of whats in main.
//
// We can also code up a lexer generator... 
//
//


// Token id? (Storing all the data, 
// Things like where it was first seen
// What it's type is, 
// etc. 

// We seem to need some things like: 
// A symbol table. 
// (make one later?) 
// Not all tokens really nee attributes... like parentheses
//
// An example of an attribute for a constant number, like: 
// 	the number 2 shows up in source code
// 	The token name is "number"
// 	and the attribute can be one of several things
// 	1.) The integer itself
// 	2.) A string representing the number
// 	3.) A pointer to the string repressenting the number
//
// 	Apparently, typical compilers use 3.) .
//
// 	The way it's looking, we'll have a base Attribute class
// 	And then we can have different subclasses
// 	*integer Attribute
// 	*string Literal Attribute
// 	*float attribute
// 	*Long int attribute
// 	*char attribute
// 	etc.
//
// Lexer can also modify the input to try and match it. 
// This is probably a bad idea, since the programmer might
// have meant something different. 
//
// We end up having to buffer the input to run our lexer on it. 
//
//

// Buffering input; 
// 1.) buffer pair. 
// 	// The pair is not the pair of pointers, 
// 	// but rather two buffers. We start off 
// 	// by reading input into buff A, and 
// 	// the next block in buff B. When buff A
// 	// is exhausted, move to buff B. When buff
// 	// B is exhausted, load input into buff A. 
// 	// Continue until the program has been processed. 
// 	// This has a very "C" style
//
// 	// Also, this might not be best for interpreters
// 	//  
// 	1.1) Read in a block of input. 
// 	1.2) Keep a pair of pointers. 
// 		one to the start of the lexeme
// 		one to the end of the lexeme
// 	1.3) Scan along until we have a match
// 	1.4) Reposition the pointers
// 	1.5) If we reach the end of the block, 
// 		load the next one. 
//
//
// Sentinel: 
// Use EOF to mark the end of the buffer, and then 
// you won't have to do both checks of whehter you're
// at the end of the buffer and whether you're at the 
// end of the input. 
//
// Dealing with whitespace: 
// We don't like Whitespace, and the parser doesn't like it. 
// So, one behavior could be to strip off whitespace. But then 
// again, languages like python might use whitespace. So, it 
// might not be that bad of an idea to have that in there. 
// after all, we do need to recognize: "\n\t\t" to make sure
// that the code is properly indented. 
// There probably would need to be a flag, that turns on 
// and off whitespace token generation. 
//
// Dealing with reserved Keywords
// Two options: 
// 	1.) They're installed in a symbol table. 
// 		when the lexer wants to return a token, 
// 		check to make sure it isn't reserved. If it is
// 		return a modified token to signify that the token
// 		isn't an id, but rather a keyword
// 	2.) Transition diagrams for keywords. 
// 		We create a new state for each successive part
// 		of the keyword. Then, return the keyword and 
// 		corresponding token name, etc. 
// Comparison of the two methods: 
// 1 is more computationally intensive (involves lookup in table, 
// 	needs values installed in table. 
// 2 might be more memory intensive with the extra states. Also 
// 	requires more inital work to create the extra states. 
//
// Implementation wise
// probably need to associate some things with each state, 
// for instance, 
// 	state number
// 	transitions
// 	pointers to the next states, iterators, etc. 
//
//
// 1.)  We can set up many different automata to try at once. 
// Then specify the order that they're run to match input. 
// If one of them works, great, pass it on. 
// If it doesn't work, then reset the forward pointer and try the next automata. 
// The issue with this one is that it seems slow... isn't there a better way? 
//
// 2.) We can try running the dfas in parallel, and then take the one that matches
// the longest prefix. 
//
// 3.) We can put all the DFA's together. Each final state will match 
// a token name. 

// DFA 
// It'll match something, 
// but then we have to have a lookahead to
// make sure we don't match something else. 
// So then what may happen is that we call: 
//
// get_next_char()
// dfa_step()
// if(dfa_fail()) {
// 	call return function of last state. 
// 	(should return the token name) 
//
// If we're trying to recognize a lot of stuff... 
// Then make it a basic list of regexes: 
//
// r1 ret name(1)
// r2 ret name(2)
// r3 ret name(3)
// ...
// rn ret name(n)
//
// we essentially start multiple transitions
// and keep going until we no longer can accept input. 
// Then, backtrack, and find the last matched state
// that is an accepting state. Move the forward pointer 
// back the number of times we go back, and return the 
// corresponding token name. 
//
// We might be running through a lot of states at the same time
// we can monitor the program to make sure it isn't too many. 


/*
class NondeterministicFA {
	//
};

*/


class TokenAttribute {
	std::string lexeme;
};

class MyToken {
	std::string name;
	// pointer to attribute
	TokenAttribute *attribute;
	
};

class LexerGenerator {

};

class MyLexer {

};

int main(int argc, char *argv[]) {

	DeterministicFA dfa;

	dfa.add_state();
	dfa.add_state();
	dfa.add_state();

	dfa.set_start(0);
	dfa.set_final(1);

	dfa.add_transition(0, 'a', 1);
	dfa.add_transition(1, 'b', 1);
	dfa.add_transition(1, 'a', 2);
	dfa.add_transition(2, 'a', 2);
	dfa.add_transition(2, 'b' ,1);

	dfa.run();

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
