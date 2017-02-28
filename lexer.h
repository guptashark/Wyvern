#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <iostream>
#include <regex>
#include <vector>

class Token {
	private:
	std::string lexeme;
	std::string symbol;

	public:
	Token(std::string lexeme, std::string symbol);
	void print();
};

class Lexer {

	private:	
	std::istream &in_file;
	std::string rgx_pattern;
	std::vector<std::string> token_symbols;
	std::regex reg;

	std::string map_sm_to_symbol(std::smatch &sm);

	public:
	Lexer(
			std::istream &in_file,
			std::string rgx_pattern,
			std::vector<std::string> token_symbols);

	std::vector<Token> getSentence();

};

#endif
