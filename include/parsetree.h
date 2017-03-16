#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include <vector>
#include <string> 

#include "lexer.h"

// TODO proper implementation
// For now, just make it with an internal flag
// indicating if it's a non-terminal or not
// later do the whole classes thing
// and make parseTreeNode an abstract base class
// TODO vector of refs or actual objects? 
class ParseTreeNode {

	private:
		// both nonterm and term nodes need this	
		bool terminal;

		// Things for just the terminal
		//Token t;
		// Things for just the non-terminal
		std::string lexeme;
		std::string symbol;
		std::vector<ParseTreeNode *> children;
		
	public:
		// constructor for terminal: 
	//	ParseTreeNode(Token terminal);

		// constructor for non-terminal:
		ParseTreeNode(std::string non_terminal);

		// Functions for terminal nodes (aka leaves)
		Token getToken();
		// The next two are probably not needed.
		//std::string getLexeme();
		//std::string getSymbol();
		
		// Functions for non-terminal nodes
		void addChild(ParseTreeNode *child);
		void makeTerminal(std::string lex);
		void print(int num_tabs);

		~ParseTreeNode();
};

/*
class ParseTree {

	ParseTreeNode root;
	public:

	ParseTree(std::string root); 
	ParseTreeNode 
	void printTree();
};
*/
#endif
