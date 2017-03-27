#ifndef __DETERMINISTICFA_H__
#define __DETERMINISTICFA_H__

#include <unordered_map>
#include <string>
#include <vector>
#include <map>

class DFAState {
	// function to call if we finish in this state. 
	//
	protected:
		unsigned int state_id;
		std::string name;
		std::unordered_map<char, DFAState *> transitions;	

	public:
		DFAState(unsigned int state_id);
		DFAState(unsigned int state_id, std::string name);
		// No destructor needed, since the DFA will call delete
		// at the end along the vector of pointers. 
	
		// Both are handy to add our needed transitions. 	
		void add_transition(char symbol, DFAState *destination);
		void add_transition(std::string symbols, DFAState *destination);
		unsigned int get_state_id();
		std::string get_state_name();

		DFAState *step(char symbol);
};

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

		// map from indexes to states. 
		std::vector<DFAState *> states;

		// name the states and map them.
		// (Helps when visualizing subset construction) 
		std::map<std::string, DFAState *> named_states;
		DFAState *current;
		DFAState *start;

		std::unordered_map<unsigned int, bool> final_states;

		DFAState *step(char c);
	
	public:
		DeterministicFA();
		void add_state();
		void add_state(std::string name);

		void add_transition(unsigned int from_state_id, char symbol, unsigned int to_state_id);
		void add_transition(std::string from_state_name, char symbol, std::string to_state_name);


		void set_start(unsigned int state_id);
		void set_start(std::string state_name);

		void set_final(unsigned int state_id);
		void set_final(std::string state_name);

		// right now it returns void, 
		// but will print out whatever is necessary to indicate what happens. 
		// in use, it should return something useful. 
		void run();
};

#endif
