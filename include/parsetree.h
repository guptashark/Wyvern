#include <vector>
#include <string> 

class ParseTreeNode {

	private: 	
	std::string symbol;

	
	public:
	ParseTreeNode(std::string symbol);

};
/*
class TerminalNode: public ParseTreeNode {

	// since we're a terminal, 
	// we hold a symbol, 
	// 	LBRACE, ID, COMMA, etc. 
	// and a lexeme
	// 	"(", "foo", ","
	// 	but we hold no children 
};

class NontermnalNode: public ParseTreeNode {
	// since we're a nontermnal,
	// we hold the nonterminal symbol. 
	// We hold a list of children. 
	private: 
		std::vector<ParseTreeNode> children;
	
};
*/


class ParseTree {

	ParseTreeNode root;
};


