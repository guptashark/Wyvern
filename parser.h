#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include <vector>
#include <set> 
#include "lexer.h"
/*
class ASTNode {
	//
}

class ASTNonterminal : public ASTNode {

}

class ASTTerimal: public ASTNode {
	
}
*/

// Build parse tree later. 
// right now, just accept input. 

// For an LL1 parser, we need to have first sets. 
// To build our table, We need to do a fair bit 
// of iterating. 


// TODO Parsing with follow sets. 
// TODO (When using no epsilons in LL1, we don't need follow sets)
// TODO We need typedefs!! Code is getting difficult to read! ie, 
// 		what is: "std::map<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::vector<std::string>>>::iterator>" This madness??
class Parser {
	private:
		// just a dummy helper in situations.
		std::string epsilon;
		std::string start_symbol;
		std::set<std::string> non_terminals;
		std::set<std::string> terminals;

		typedef std::pair<std::string, std::vector<std::string>> rule;
		typedef std::pair<std::string, std::string> lookup_table_key;
		typedef std::map<lookup_table_key, std::vector<rule>::iterator>::iterator lookup_table_iterator;

	//	std::map<std::pair<std::string, std::string>, int> rule_lookup;
		std::map<lookup_table_key, std::vector<rule>::iterator> rule_lookup;
		std::vector<rule> rules;
		std::map<std::string, std::set<std::string>> first_sets;

		// This is an internal printing function. 
		void printRule(std::vector<rule>::iterator);
		// This is a function, internally used. It's recursive. 
		std::set<std::string> createFirstSets(std::string S);

	public:
		Parser(std::string filename);
		void parse(std::vector<Token> sentence);
		void printRules();
		void createLookupTable();
		void printFirstSets();
		void printLookupTable();

		// for testing right now... 
		void parseInput(std::vector<std::string>);
	
};

#endif
