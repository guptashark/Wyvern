#include <iostream>
#include <stack>
#include <unordered_map>
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

class DFAState {
	// function to call if we finish in this state. 
	//
	protected:
		unsigned int state_id;
		std::unordered_map<char, DFAState *> transitions;	

	public:
		DFAState(unsigned int state_id);
		// No destructor needed, since the DFA will call delete
		// at the end along the vector of pointers. 
	
		// Both are handy to add our needed transitions. 	
		void add_transition(char symbol, DFAState *destination);
		void add_transition(std::string symbols, DFAState *destination);
		unsigned int get_state_id();

		DFAState *step(char symbol);
};

DFAState::DFAState(unsigned int state_id): state_id(state_id) {}

void DFAState::add_transition(char symbol, DFAState *destination) {
	// check if this transition already exists in the table. 
	
	auto it = transitions.find(symbol);
	if(it != transitions.end()) {
		// throw an exception 
		// TODO actually throw the exception. 
		cout << "Err: " << symbol << " already mapped to state: ";
		cout << it->second->get_state_id();
		return;
	}

	// do the check just to be sure
	if(it == transitions.end()) {
		std::pair<char, DFAState *> to_add(symbol, destination);
		transitions.insert(to_add);
	}

}

void DFAState::add_transition(std::string symbols, DFAState *destination) {

	// Catch exceptions as they come along. 
	for(auto it = symbols.begin(); it != symbols.end(); it++) {
		add_transition(*it, destination);
		// should catch exceptions here. 
	}
}

unsigned int DFAState::get_state_id() {
	return state_id;
}

// Might later change what happens when there is no next step... 
// But it also makes sense to have a map from state id to functions
// that way the dfa itself can hold all that for later processing
// as compared to the children nodes... 
// we can change this later with the sort of flexible structure
// we currently have. 
DFAState *DFAState::step(char symbol) {
	auto it = transitions.find(symbol);
	if(it == transitions.end()) {
		return NULL;
	} else {
		return it->second;
	}
}
/*
class NFAState {
	//
};
*/
class DeterministicFA {

	// vector of pointers to states. 
	// should probably allow creation from NFA... 
	// and more sophisticated creation techniques.
	//
	// the caller currently needs to know 
	// which state they're on. 
	// This is really only because we don't yet
	// have an automated way of building the dfa
	// otherwise we probably would have that in here. 
	private:
		unsigned int num_states;
		std::vector<DFAState *> states;
		DFAState *current;
		DFAState *start;

		std::unordered_map<unsigned int, bool> final_states;

		DFAState *step(char c);
	
	public:
		DeterministicFA();
		void add_state();
		void add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id);
		void set_start(unsigned int state_id);

		void set_final(unsigned int state_id);

		// right now it returns void, 
		// but will print out whatever is necessary to indicate what happens. 
		// in use, it should return something useful. 
		void run();
};

DeterministicFA::DeterministicFA(): num_states(0), current(NULL), start(NULL) {}

DFAState *DeterministicFA::step(char c) {
	if(current == NULL) {
		return NULL;
	}

	current = current->step(c);
	return current;
}

void DeterministicFA::add_state() {
	DFAState *to_add = new DFAState(num_states);
	states.push_back(to_add);
	num_states++;
}

void DeterministicFA::add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id) {
	states[from_state_id]->add_transition(symbol, states[to_state_id]);
}

void DeterministicFA::set_start(unsigned int state_id) {
	// if the start has already been set, throw an error
	// TODO actually throw the error. 
	if(start == NULL) {
		start = states[state_id];
	} else {
		cout << "Err, start state is already set." << endl;
	}
}	

void DeterministicFA::set_final(unsigned int state_id) {

	// the unordered map seems like a bad implementation
	// for checking if a state is final... 
	// TODO find a better way to check for if a stat is final.
	std::pair<unsigned int, bool> to_set(state_id, true);
	final_states.insert(to_set);
}

void DeterministicFA::run() {
	current = start;
	std::string seen_string;
	std::stack<unsigned int> visited_states;

	cout << "Please start." << endl;
	
	while(current != NULL) {
		cout << "In state: " << current->get_state_id();
		cout << endl;

		visited_states.push(current->get_state_id());

		char c;
		cin >> c;
		current = current->step(c);
		seen_string.push_back(c);
	}

	// Since we're now at the end...
	// see how much we need to push back to get to an accepting state. 
	bool accepter_found = false;

	while(!accepter_found) {
		unsigned int top = visited_states.top();
		auto iter = final_states.find(top);
		if(iter == final_states.end()) {

			visited_states.pop();
			seen_string.pop_back();
		} else {
			accepter_found = true;
		}
	}

	cout << "The accepted string is: " << seen_string << endl;
}

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
