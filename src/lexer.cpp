
#include "Lexer.h"

using std::cout;
using std::string;
using std::vector;

/* Token functions */
Token::Token(string lexeme, string symbol):
	lexeme(lexeme), symbol(symbol) {}

string Token::getSymbol() {
	return symbol;
}

void Token::print() {
	cout << lexeme << ": " << symbol;
}

Lexer::Lexer(
	std::istream &in_file, 
	string rgx_pattern,
	vector<string> token_symbols):
		in_file(in_file), 
		rgx_pattern(rgx_pattern),
		reg(rgx_pattern),
		token_symbols(token_symbols) {}

string Lexer::map_sm_to_symbol(std::smatch &sm) {
	
	int counter = 0;
	for(unsigned it = 1; it <  sm.size(); it++) {
		if((sm[it].str()).empty()) {
			counter++;
		} else {
			break;
		}
	}

	return token_symbols[counter];
}



vector<Token> Lexer::getSentence() {

		string input;
		getline(in_file, input);
		vector<Token> sentence;

		string input_suffix = input;
		string input_prefix;

		string ret;

		while(!input_suffix.empty() && input_prefix.empty()) {
			std::smatch sm;
			std::regex_search(input_suffix, sm, reg);
			input_prefix = sm.prefix();
			if(input_prefix.empty()) {
				ret = map_sm_to_symbol(sm);
				sentence.push_back(Token(sm[0], ret));
			}

			input_suffix = sm.suffix();
		}

		return sentence;
}





