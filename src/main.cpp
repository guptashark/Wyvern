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

using namespace std;

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


// in order to make this well formed, 
// We're going to try working with lists, 
// because we can make them ordered. 

class NFAState {
	// function to call if we finish in this state. 
	//
	protected:
		unsigned int state_id;
		std::string name;
		bool is_named;

		// Sentinel value for helping speed up the algo
		// So that we don't have to compute eps_close
		// for the same state multiple times. 
		bool computed_eps_closure;

		// We need a set of pointers.
		// preferably ordered...
		// because we want precedence matching? 
		// right now, just a set. 
		std::unordered_map<char, std::set<NFAState *>> transitions;

		// need something for epsilon transitions. 
		
		// This one is for just basic transitions
		std::set<NFAState *> epsilon_transitions;

		// This one is for the full closure that we find. 
		std::set<NFAState *> eps_closure;

	public:
		NFAState(unsigned int state_id);
		NFAState(std::string name, unsigned int state_id);
		// No destructor needed, since the DFA will call delete
		// at the end along the vector of pointers. 
	
		// Both are handy to add our needed transitions. 	
		void add_transition(char symbol, NFAState *destination);
		void add_transition(std::string symbols, NFAState *destination);

		// add an epsilon transition
		void add_epsilon_transition(NFAState *destination);

		std::set<NFAState *> get_eps_closure();

		unsigned int get_state_id();

		// Issue, we're returning a copy... right? 
		// TODO make sure this is ok. 
		// Maybe use a reference? 
		std::set<NFAState *> step(char symbol);

		// We also need to get states on e transitions
		std::set<NFAState *> epsilon_step();



		void print_info();
		void print_identifiers();
};

NFAState::NFAState(string name, unsigned int state_id): state_id(state_id), name(name), is_named(true), computed_eps_closure(false) {}

// TODO
// probably need better naming. 
std::set<NFAState *> NFAState::get_eps_closure() {

//	cout << "computing " << state_id << " e-close" << endl;
	if(computed_eps_closure) {
//		cout << "Already computed, returning " << state_id << " e-close" << endl;
		return eps_closure;
	}

//	cout << "Not already computed, computing." << endl;

	// otherwise, we need to construct it. 

	set<NFAState *> eps_close(epsilon_transitions);

	// loop is just for printing purposes: 
/*
	cout << "Already has: ";
	for(auto i = eps_close.begin(); i != eps_close.end(); i++) {
		cout << (*i)->get_state_id() << " ";
	}
	cout << "In e-close" << endl;
*/

	for(auto i = epsilon_transitions.begin(); i != epsilon_transitions.end(); i++) {
		set<NFAState *> child_eps((*i)->get_eps_closure());
		for(auto j = child_eps.begin(); j != child_eps.end(); j++) {
//			cout << "Adding in e-close: " << (*j)->get_state_id() <<  endl;
			eps_close.insert(*j);
		}
	}

	eps_closure = eps_close;
	computed_eps_closure = true;
//	cout << "finally computed e-close for " << state_id << " returning." << endl;
	return eps_closure;
}
	


void NFAState::print_identifiers() {
	if(is_named) {
		cout << "(" << name << ", " << state_id << ")";
	} else {
		cout << "(" << state_id << ")";
	}
}

class NonDeterministicFA {

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
		std::vector<NFAState *> states;
		
		// We need a set of current states...
		std::set<NFAState *> current;
		NFAState *start;

		std::unordered_map<unsigned int, bool> final_states;

		std::set<NFAState *> epsilon_step();
		std::set<NFAState *> step(char c);
	
	public:
		NonDeterministicFA();
		void add_state(std::string name);
		void add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id);
		void add_epsilon_transition(unsigned int from_state_id, unsigned int to_state_id);
		void set_start(unsigned int state_id);

		void set_final(unsigned int state_id);

		// right now it returns void, 
		// but will print out whatever is necessary to indicate what happens. 
		// in use, it should return something useful. 
		void print_info();
		void compute_epsilon_closures();
		void run();
};

NonDeterministicFA::NonDeterministicFA(): num_states(0), start(NULL) {}

void NonDeterministicFA::compute_epsilon_closures() {

	for(auto i = states.begin(); i != states.end(); i++) {
		(*i)->get_eps_closure();
	}
}

void NonDeterministicFA::add_epsilon_transition(unsigned int from_state_id, unsigned int to_state_id) {
	states[from_state_id]->add_epsilon_transition(states[to_state_id]);
}

void NonDeterministicFA::print_info() {

	for(auto it = states.begin(); it != states.end(); it++) {

		(*it)->print_info();
	}
}


NFAState::NFAState(unsigned int state_id): state_id(state_id), is_named(false), computed_eps_closure(false) {}

void NFAState::add_transition(char symbol, NFAState *destination) {
	// check if this transition already exists in the table. 

	// Check if there's already a list there. 
	// If there isn't put this item in the list. 
	// Then insert the char and list into the map.
	// If there is, then just add the destination to the list.
	auto it = transitions.find(symbol);
	if(it != transitions.end()) {
		it->second.insert(destination);
		return;
	}

	if(it == transitions.end()) {
		// make a list. 
		std::set<NFAState *> my_set;
		my_set.insert(destination);
		std::pair<char, set<NFAState *>> to_add(symbol, my_set);
		transitions.insert(to_add);
	}
}

void NFAState::add_transition(std::string symbols, NFAState *destination) {

	// Catch exceptions as they come along. 
	for(auto it = symbols.begin(); it != symbols.end(); it++) {
		add_transition(*it, destination);
		// should catch exceptions here. 
	}
}

void NFAState::add_epsilon_transition(NFAState *destination) {
	epsilon_transitions.insert(destination);
}

unsigned int NFAState::get_state_id() {
	return state_id;
}

void NFAState::print_info() {

	// for now just iterate through set. 
	// later use a list. 
	if(is_named) {
		cout << name << ", ";
	}

	cout << "State id: " << state_id << endl;	
	for(auto i = transitions.begin(); i != transitions.end(); i++) {
		cout << i->first << ": ";
		for(auto j = i->second.begin(); j != i->second.end(); j++) {
			(*j)->print_identifiers();
			cout << " ";
		}

		cout << endl;
	}

	cout << "epsilon: ";
	for(auto i = epsilon_transitions.begin(); i != epsilon_transitions.end(); i++) {
		cout << (*i)->get_state_id() << " ";
	}
	cout << endl;

	cout << "Epsilon closure: ";
	for(auto i = eps_closure.begin(); i != eps_closure.end(); i++) {
		cout << (*i)->get_state_id() << " ";
	}

	cout << endl << endl;
}

/*
// Might later change what happens when there is no next step... 
// But it also makes sense to have a map from state id to functions
// that way the dfa itself can hold all that for later processing
// as compared to the children nodes... 
// we can change this later with the sort of flexible structure
// we currently have. 
list<NFAState *> NFAState::step(char symbol) {
	auto it = transitions.find(symbol);
	if(it == transitions.end()) {
		return NULL;
	} else {
		return it->second;
	}
}
*/

/*
list<NFAState *> NFAState::epsilon_step() {
	return epsilon_transitions;
}
*/

/*
set<NFAState *> NonDeterministicFA::step(char c) {
	if(current.empty()) {
		std::set<NFAState *> empty_list;
		return empty_list;
	}

	current = current->step(c);
	return current;
}
*/
void NonDeterministicFA::add_state(string state_name = string()) {

	NFAState *to_add = NULL;

	if(state_name.empty()) {
		to_add = new NFAState(num_states);
	} else {
		to_add = new NFAState(state_name, num_states);
	}

	states.push_back(to_add);
	num_states++;
}

void NonDeterministicFA::add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id) {
	states[from_state_id]->add_transition(symbol, states[to_state_id]);
}

void NonDeterministicFA::set_start(unsigned int state_id) {
	// if the start has already been set, throw an error
	// TODO actually throw the error. 
	if(start == NULL) {
		start = states[state_id];
	} else {
		cout << "Err, start state is already set." << endl;
	}
}	

void NonDeterministicFA::set_final(unsigned int state_id) {

	// the unordered map seems like a bad implementation
	// for checking if a state is final... 
	// TODO find a better way to check for if a stat is final.
	std::pair<unsigned int, bool> to_set(state_id, true);
	final_states.insert(to_set);
}

// We're facing an issue
// When we are running
// We check what state we're in
// We then get every possible state we can be in by epsilon transition. 
// 
// Lets assume we have this set. Ie, for instance: 
// A eps B
// B eps C
// A d D
// B c E
// A c F
//
// Then , if A is the start state, we would get the set: 
// {A, B, C} as the ones we can possibly be in. 
// Then, we apply the transition on symbol c. 
// Our new set becomes: 
// {F, E} 
//
// Since C can't go to anything on input symbol c
//
// And then we start our algorithm again from {E, F}
// Add in all the states we could be in from eps transitions
// and then read the next input character. 

// How do we do the first part? Constructing the set
// of all states we can get to with just eps-t
//
/*
void NonDeterministicFA::run() {
	current.push_back(start);
	std::string seen_string;
	std::stack<std::list<NFAState *>> visited_states;

	cout << "Please start." << endl;
	
	while(!current.empty()) {
		cout << "In states " ;
		for(auto iter = current.begin(); iter != current.end(); iter++) {
			cout << current->get_state_id() << " ";
		}
		cout << endl;

		visited_states.push(current);

		char c;
		cin >> c;

		// now we need to do shenanigans.
		// build a set of states we can get to from e-transitions
		// if we can get from A to B with e-t, then 
		
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
