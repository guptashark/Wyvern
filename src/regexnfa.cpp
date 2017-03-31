
/* There needs to be some way to properly construct NFA's from regex. 
 *
 * WIth regex, we have something cool: 
 * We don't care about the id of the state
 * we don't care about the names of the states, 
 * they are literally created through regex. 
 *
 * it's like, full space efficiency since the only thing we need: 
 *
 * we need to know the following: 
 * 	1: states
 * 	2: transition function
 * 	3: start state
 * 	4: final states. 
 *
 * Now, we will decide how to implement this after making it a point
 * that we need to implement regex. 
 * ANd that we're going to be building up regex from basically first principles. 
 *
 * example: concatenation: 
 * 	- get the start state of nfa2
 * 	- unattatch the start of nfa2
 * 	- add an e transition from all final states of nfa1 to nfa2
 * 	- the final state of nfa1 is no longer final. 
 * 	- done
 *
 * example: union: 
 * 	- construct a new start state. 
 * 	- unattatch the two start states of nfa1 and nfa2
 * 	- epsilon transition them to a new start state. 
 * 	- all final states of both nfa's now are no longer final, and lead to a new final state. 
 * 	?? Does this still allow us to recognize tokens we're interested in?
