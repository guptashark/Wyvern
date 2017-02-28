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

class Parser {
	private:
		std::string start_symbol;
		std::set<std::string> non_terminals;
		std::set<std::string> terminals;

		std::map<std::pair<std::string, std::string>, int> rule_lookup;
		std::vector<std::pair<std::string, std::vector<std::string>>> rules;

		std::map<std::string, std::set<std::string>> first_sets;
		std::set<std::string> createFirstSets(std::string S);

	public:
		Parser(std::string filename);
		void parse(std::vector<Token> sentence);
		void printRules();
		void printFirstSets();
	
};

#endif
