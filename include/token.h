#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

// a token that wraps up several things. 
class Token {
	// generate the token names from a lex file. 

	private: 
	// the string, as it appears in the source program. 	
	std::string lexeme;

	// the token name, ie:
	// is it an identifier, 
	// is it a relational operator
	// is it a parenthesis
	// is it a number
	std::string token_name;

	// we don't really know what this is going to be, 
	// but it seems like we'll have to implement 
	// a symbol table. 
	// for now... don't have an attribute value. 
	// AttributeValue av;

	public: 
	

};

#endif
