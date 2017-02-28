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



class Parser {
	private:
		std::set<std::string> non_terminals;
		std::set<std::string> terminals;

		std::map<std::pair<std::string, std::string>, int> rule_lookup;
		std::vector<std::pair<std::string, std::vector<std::string>>> rules;

	public:
		Parser(std::string filename);
		void parse(std::vector<Token> sentence);
		void printRules();
	
};

#endif
