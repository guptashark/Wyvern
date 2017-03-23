#include <iostream>
#include "nondeterministicfa.h"

using namespace std;

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


