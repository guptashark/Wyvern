#include "parsetree.h"

ParseTreeNode::ParseTreeNode(string symbol): symbol(symbol) {}

void ParseTreeNode::addchild(ParseTreeNode child) {
	children.push_back(child);
}


