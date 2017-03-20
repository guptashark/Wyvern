#include <iostream>
#include <stack>
#include "deterministicfa.h"

using namespace std;

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

