#include <iostream>
#include <regex>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {

	(void)argc;
	(void)argv;

	string input;
	string pattern ("[[:digit:]]");

	std::regex digit(pattern);

	while(true) {

		cin >> input;
		if(!cin) return 0;
		if(input == "q") return 0;
		if(std::regex_match(input, digit)) {

			cout << "Matched" << endl;
		} else {
			cout << "Unmatched" << endl;
		}
	}



	return 0;
}





