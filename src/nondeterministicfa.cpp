#include <stack> 
#include <utility>
#include <iostream>
#include <list> 
#include <sstream>
#include "nondeterministicfa.h"

using namespace std;

NFAState::NFAState(string name, unsigned int state_id): 
	state_id(state_id), 
	name(name), 
	is_final(false), 
	computed_eps_closure(false)
{}

std::set<NFAState *> NFAState::get_eps_closure() {

	// patch: 
	// The element itself must be in the epsilon closure! 

	if(computed_eps_closure) {
		return eps_closure;
	}

	set<NFAState *> eps_close(epsilon_transitions);

	for(auto i = epsilon_transitions.begin(); i != epsilon_transitions.end(); i++) {
		set<NFAState *> child_eps((*i)->get_eps_closure());
		for(auto j = child_eps.begin(); j != child_eps.end(); j++) {
			eps_close.insert(*j);
		}
	}

	// the patch itself.
	eps_close.insert(this);

	eps_closure = eps_close;
	computed_eps_closure = true;
	return eps_closure;
}
	
void NFAState::set_as_final() {
	is_final = true;
}

void NFAState::set_as_final(string arv) {
	is_final = true;
	accepting_return_val = arv;
}

string NFAState::get_arv() {
	return accepting_return_val;
}

bool NFAState::get_is_final() {
	return is_final;
}

void NFAState::print_identifiers() {
	if(!name.empty()) {
		cout << "(" << name << ", " << state_id << ")";
	} else {
		cout << "(" << state_id << ")";
	}
}

NonDeterministicFA::NonDeterministicFA(): start(NULL), sr(NULL) {}

NonDeterministicFA::NonDeterministicFA(list<string> los): start(NULL), sr(NULL) {

	// make the start state. 
	add_state("start");
	set_start(0);
	for(auto i = los.begin(); i != los.end(); i++) {
		string s(*i);
		int current_len = 0;
		int len = s.size();
		string s_name(s);
		s_name.push_back('-');
		add_state(s_name);
		cout << "Adding state: " << s_name << endl;
		add_epsilon_transition("start", s_name);
		for(auto j = s.begin(); j != s.end(); j++) {
			string next_state = s_name;
			next_state.push_back(*j);
			add_state(next_state);
			cout << "Adding state: " << next_state << endl;
			add_transition(s_name, *j, next_state);
			s_name.push_back(*j);
			current_len++;
			if(current_len == len) {
				set_final(next_state, "keyword-" + s);
			}
		}

	}
}

void NonDeterministicFA::add_epsilon_transition(std::string from_state_name, std::string to_state_name) {
	named_states[from_state_name]->add_epsilon_transition(named_states[to_state_name]);
}

void NonDeterministicFA::set_source_reader(SourceReader *sr_p) {
	sr = sr_p;
}

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


NFAState::NFAState(unsigned int state_id): state_id(state_id), computed_eps_closure(false) {}

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
	if(!name.empty()) {
		cout << name << ", ";
	}

	cout << "State id: " << state_id << endl;	
	if(is_final) cout << "is final" << endl;
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
*/
set<NFAState *> NFAState::step(char symbol) {
	std::set<NFAState *> ret;
	auto it = transitions.find(symbol);
	if(it == transitions.end()) {
		return ret;
	} else {
		ret = it->second;
		return ret;
	}
}

void NonDeterministicFA::add_state(string state_name = string()) {

	NFAState *to_add = NULL;

	if(state_name.empty()) {
		to_add = new NFAState(states.size());
	} else {
		to_add = new NFAState(state_name, states.size());
		pair<string, NFAState *> to_insert(state_name, to_add);
		named_states.insert(to_insert);
	}

	states.push_back(to_add);
}

void NonDeterministicFA::add_transition(string from_state_name, string symbols, string to_state_name) {
	named_states[from_state_name]->add_transition(symbols, named_states[to_state_name]);
}

void NonDeterministicFA::add_transition(string from_state_name, char symbol, string to_state_name) {
	named_states[from_state_name]->add_transition(symbol, named_states[to_state_name]);
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
	// find the state to set as final. 
	states[state_id]->set_as_final();	
//	std::pair<unsigned int, bool> to_set(state_id, true);
//	final_states.insert(to_set);
}

void NonDeterministicFA::set_final(string state_name) {
	named_states[state_name]->set_as_final();
}

void NonDeterministicFA::set_final(string state_name, string arv) {
	named_states[state_name]->set_as_final(arv);
}

string NonDeterministicFA::state_set_to_string(set<NFAState *> &s) {

	// probably shouldn't even get here... 
	if(s.empty()) {
		string result;
		return result;
	}
	// first get all the items into a linked list. 
	// sort the list. 
	// turn the list into a string. 
	list<unsigned int> l;
	for(auto i = s.begin(); i != s.end(); i++) {
		l.push_back((*i)->get_state_id());
	}

	l.sort();

	// use stringstreams to turn numbers into strings... 
	stringstream ss;
	ss << "{";
	for(auto i = l.begin(); i != l.end(); i++) {
		ss << *i << ", ";
	}

	// now read this all into a string. 
	string result = ss.str();
	// lop off the last little bit. 
	// (We'll only ever get here if s isn't empty, so at least one state exists in the set. 
	result.pop_back();
	result.pop_back();
	result.push_back('}');
	return result;
}

DeterministicFA NonDeterministicFA::convert_to_dfa() {
	// create the dfa as we go. 
	
	// make the set of states that is the start set of the dfa. 
	// turn that set into one DFAState. 
	// add the dfa state into the dfa. 
	// also add the dfa state to a queue of marked dfa states. 
	// move on a symbol. 
	// generate the set of states we get from moving on the symbol. 
	// turn that set into a dfa state. 
	// check and see if that specific state already exists in the dfa
 	DeterministicFA d;

	// create the start state for the dfa...
	set<NFAState *> T(start->get_eps_closure());
	// turn the set into a string. 
	string state_name(state_set_to_string(T));
	d.add_state(state_name);

	cout << state_name << endl;

	return d;	

}


// newish behavior
// if attatched to a source reader, 
// read from it instead of from stdin.
void NonDeterministicFA::run() {
	std::set<NFAState *> current;
	current.insert(start);
	std::string seen_string;
	std::stack<std::set<NFAState *>> visited_states;

	cout << "Please start." << endl;
	
	while(!current.empty()) {
		cout << "In states " ;

		std::set<NFAState *> X;
		// build our epsilon closure of current. 
		// reassign to current. 
		for(auto i = current.begin(); i != current.end(); i++) {
			std::set<NFAState *> ret;

			ret = (*i)->get_eps_closure();
			for(auto j = ret.begin(); j != ret.end(); j++) {
				X.insert(*j);
			}
		}
		// get the eps_closures, X. 
		
		for(auto iter = X.begin(); iter != X.end(); iter++) {
			cout << (*iter)->get_state_id() << " ";
		}
		cout << endl;

		//visited_states.insert(current);

		char c;
		if(sr == NULL) {
			cin >> c;
		} else {
			c = sr->next_char();
		}

		// now we need to do shenanigans.
		// build a set of states we can get to from e-transitions
		// if we can get from A to B with e-t, then 
	
		std::set<NFAState *> Y;
		for(auto i = X.begin(); i != X.end(); i++) {
			std::set<NFAState *> ret;
			ret = (*i)->step(c);
			for(auto j = ret.begin(); j != ret.end(); j++) {
				Y.insert(*j);
			}
		}

		current = Y;
		seen_string.push_back(c);
		visited_states.push(current);
	}

	// Since we're now at the end...
	// see how much we need to push back to get to an accepting state. 
	bool accepter_found = false;
	NFAState *acceptor = NULL;
	while(!accepter_found && !visited_states.empty()) {
		// get the top set of states. 
		std::set<NFAState *> top = visited_states.top();
		// check if any of them are final states. 
		bool contains_final_state = false;
	
		for(auto i = top.begin(); i != top.end(); i++) {
			if((*i)->get_is_final()) {
				contains_final_state = true;
				acceptor = *i;
			}
		}
		if(!contains_final_state) {
			
			visited_states.pop();
			seen_string.pop_back();
			// if we have a source reader
			if(sr != NULL) {
				sr->rewind_input();
			}

		} else {
			accepter_found = true;
		}
	}

	if(acceptor == NULL) {
		cout << "acceptor state is null..." << endl;

	}

	cout << "The token name is: " << acceptor->get_arv() << endl;
	cout << "The accepted string is: \"" << seen_string << "\"" << endl;
}
