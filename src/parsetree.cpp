
#include "parsetree.h"

using namespace std;
// Constructor for terminals
//ParseTreeNode::ParseTreeNode(Token t): terminal(true), t(t) {}

// Constructor for non-terminals
ParseTreeNode::ParseTreeNode(string symbol): 
	terminal(false),
	symbol(symbol)
	{}

void ParseTreeNode::addChild(ParseTreeNode *child) {
	children.push_back(child);
}

void ParseTreeNode::makeTerminal(string lex) {
	lexeme = lex;
	terminal = true;
}

void ParseTreeNode::print(int num_tabs) {
	// just let tabs be 4 spaces. 

	for(int i = 0; i < num_tabs; i++) {
		cout << "   ";
	}

	if(terminal) {
		cout << lexeme << ": " << symbol << endl;
		return;
	}

	// otherwise, we're not a terminal
	cout << symbol << endl;
	for(auto i = children.begin(); i!= children.end(); i++) {
		(*i)->print(num_tabs + 1);
	}
}
