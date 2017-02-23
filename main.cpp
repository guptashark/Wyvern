#include <iostream>
#include <regex>
#include <string>
#include <vector>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void single_pattern_loop(string pattern) {
	string input;
	std::regex reg(pattern);

	while(true) {
		cin >> input;
		if(!cin) return;
		if(input == "q") return;

		if(std::regex_match(input, reg)) {

			cout << "Matched" << endl;
		} else {
			cout << "Unmatched" << endl;
		}
	}
}

/* TODO return a vector, or have one as a param? */
void tokenize_input(vector<string> &sv, string input, string rgx_pattern) {
	
	std::regex reg(rgx_pattern);
	string input_suffix = input;
	string input_prefix;
	while(!input_suffix.empty() && input_prefix.empty()) {
		std::smatch sm;
		std::regex_search(input_suffix, sm, reg);
		input_prefix = sm.prefix();
		if(input_prefix.empty()) {
			sv.push_back(sm[0]);
		}
		input_suffix = sm.suffix();
	}
}

void print_str_vector(vector<string> &sv) {
	std::vector<string>::iterator it;

	for(it = sv.begin(); it != sv.end(); it++) {
		cout << "[" << *it << "] ";
	}

	cout << endl;
}

/* What we will try: 
 * 1.) get the entire string in a line. 
 * 2.) Run regex that matches all possible tokens. 
 * 3.) If it works, split string, push on a vector, keep going. 
 * 4.) Run the regex of each possible thing against the lexeme until it matches. 
 */
int main(int argc, char *argv[]) {

	(void)argc;
	(void)argv;

	string input;
	string digit ("[[:digit:]]");

	/* equal, period, comma, lparen, rparen */
	string single_symbols ( "(=)|(\\.)|(\\,)|(\\()|(\\))");
	string identifier ("([_[:alnum:]]+)|");
	string whitespace ("([[:space:]]+)|");

	
	string rgx_pattern = identifier + whitespace + single_symbols;
	/*		([_[:alnum:]]+)|(=)|([[:sace:]]+)|(\\.)|(\\,)|(\\()|(\\))); */



	std::regex reg(rgx_pattern);
/*
	while(true) {
		cout << "Enter a string to be matched: ";
		getline(cin, input);
		std::smatch sm;
		std::regex_search(input, sm, reg);	

		std::smatch::iterator it;
		std::cout << "prefix=[" << sm.prefix() << "], ";
		for(it = sm.begin(); it != sm.end(); it++) {
			cout << "[" << *it << "], ";
		}
		std::cout << "suffix=[" << sm.suffix() << "]\n";
	}	
*/
	while(true) {
		getline(cin, input);
		vector<string> sv;
		tokenize_input(sv, input, rgx_pattern);
		print_str_vector(sv);
	}
	
	return 0;
}





