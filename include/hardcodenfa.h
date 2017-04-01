#ifndef __NONDETERMINISTICFA_H__
#define __NONDETERMINISTICFA_H__

#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>

#include "deterministicfa.h"
#include "source_reader.h"


class NFAState {
	// function to call if we finish in this state. 
	//
	protected:
		unsigned int state_id;
		std::string name;
		bool is_final;

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

		// if this is a final state, we want to return something. 
		// later on, this will be a function that is called
		// but right now, have it return a string, denoting maybe like, 
		// token name. 
		// then the run of the nfa itself will return something like a 
		// pair. but right now, lets get the functionality in place. 
		// later, instead of returning a string, we can probably 
		// install a function or do something more complex. 
		std::string accepting_return_val;

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

		// the second one is here temporarily
		// so that the state has control over what happens
		// when we land in it. 	
		void set_as_final();
		void set_as_final(std::string arv); // (arv == accepting return val) 

		
		bool get_is_final();

		// when we know that this state is a final state, 
		// we want to get it's arv (accepting return val) 
		std::string get_arv();

		// We also need to get states on e transitions
		std::set<NFAState *> epsilon_step();

		// for debugging stuff
		void print_info();
		void print_identifiers();
};


class HardCodeNFA {

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
		std::vector<NFAState *> states;
		
		// We need a set of current states...
		//std::set<NFAState *> current;
		NFAState *start;
		
		// The attatched source reader. 
		// A source reader is created for each file to read. 
		SourceReader *sr;

		//std::set<NFAState *> epsilon_step();
		//std::set<NFAState *> step(char c);
		std::map<std::string, NFAState *> named_states;

		
	public:
		HardCodeNFA();

		// a temporary constructor, to help initiate the idea 
		// of generating an NFA vs hard coding the states. 
		// TODO
		// improve this to work with regex, and work with Lex like syntax. 
		HardCodeNFA(std::list<std::string> los);

		HardCodeNFA(HardCodeNFA nfa1, char operation, HardCodeNFA nfa2);
		HardCodeNFA(HardCodeNFA nfa, char closure);

		void add_state(std::string name);
		void add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id);
		void add_transition(std::string from_state_name, char symbol, std::string to_state_name);
		void add_transition(std::string from_state_name, std::string symbols, std::string to_state_name);

		void add_epsilon_transition(unsigned int from_state_id, unsigned int to_state_id);
		void add_epsilon_transition(std::string from_state_name, std::string to_state_name);
		void set_start(unsigned int state_id);

		void set_final(unsigned int state_id);
		void set_final(std::string state_name);

		// this one here is to install into the state what we should get back when we accept on that state. 
		void set_final(std::string state_name, std::string arv);

		// right now it returns void, 
		// but will print out whatever is necessary to indicate what happens. 
		// in use, it should return something useful. 
		void print_info();
		void compute_epsilon_closures();
		std::pair<std::string, std::string> run();

		std::string state_set_to_string(std::set<NFAState *> &s);
		DeterministicFA convert_to_dfa();

		// we want to run this nfa multiple times. 
		// we also want to attatch it to a source reader. 
		// we want to be able to attatch it to different readers, 
		// and maintain the same nfa. 
		void set_source_reader(SourceReader *sr_p);
};



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

#endif
