#include <iostream>
#include <stack>
#include "deterministicfa.h"

using namespace std;

// constructors
DFAState::DFAState(unsigned int state_id): state_id(state_id), name(""), is_final(false) {}
DFAState::DFAState(unsigned int state_id, string name): state_id(state_id), name(name), is_final(false) {}

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

bool DFAState::get_is_final() {
	return is_final;
}

void DFAState::set_as_final() {
	// TODO
	// throw exception when already set as final. 
	is_final = true;
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

string DFAState::get_state_name() {
	return name;
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

void DeterministicFA::add_state(string name) {

	// check if the name already exists
	auto iter = named_states.find(name); 
	if(iter != named_states.end()) {
		cout << "Error, state name: \"" << name << "\" already exits." << endl;
	}


	DFAState *to_add = new DFAState(num_states, name);
	states.push_back(to_add);
	
	// should do error checking in case state with that name already exists. 
	pair<string, DFAState *> to_insert(name, to_add);
	named_states.insert(to_insert);
	num_states++;
}

void DeterministicFA::add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id) {
	states[from_state_id]->add_transition(symbol, states[to_state_id]);
}

void DeterministicFA::add_transition(string from_state_name, char symbol, string to_state_name) {
	named_states[from_state_name]->add_transition(symbol, named_states[to_state_name]);
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

void DeterministicFA::set_start(string state_name) {
	// error checking... 
	if(start == NULL) {
		start = named_states[state_name];
	} else {
		cout << "Err, start state is already set." << endl;
	}
}

void DeterministicFA::set_final(unsigned int state_id) {

	// the unordered map seems like a bad implementation
	// for checking if a state is final... 
	// TODO find a better way to check for if a stat is final.
	
	states[state_id]->set_as_final();
}

void DeterministicFA::set_final(string state_name) {
	
	unsigned int state_id = named_states[state_name]->get_state_id();
	set_final(state_id);
}

void DeterministicFA::run() {
	current = start;
	std::string seen_string;
	std::stack<DFAState *> visited_states;

	cout << "Please start." << endl;
	
	while(current != NULL) {
		cout << "In state: " << current->get_state_id();
		cout << " - " << current->get_state_name();
		cout << endl;

		visited_states.push(current);

		char c;
		cin >> c;
		current = current->step(c);
		seen_string.push_back(c);
	}

	// Since we're now at the end...
	// see how much we need to push back to get to an accepting state. 
	bool accepter_found = false;

	while(!accepter_found && !visited_states.empty()) {
		DFAState *top = visited_states.top();

		if(!top->get_is_final()) {
			visited_states.pop();
			seen_string.pop_back();
		} else {
			accepter_found = true;
		}
	}

	if(visited_states.empty()) {
		cout << "No final state was ever reached." << endl;
	} else {
		cout << "The accepted string is: " << seen_string << endl;
	}
}
