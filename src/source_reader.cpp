#include <iostream>
#include "source_reader.h"

using namespace std;

SourceReader::SourceReader(): from_shell(true) {}


SourceReader::SourceReader(string src_file) {

	source.open(src_file);	

}

char SourceReader::next_char() {
//	if(from_shell) {
//		char c;
//		cin >> c;
//		return c;
//	} else {
	char c = source.get();
	if(source.eof()) {
		//cout << "Returning the null character" << endl;
		return '\0';
	} else {
		return c;
	}
}

// issues when working with this... 
// source reader needs to store all of the characters we're getting... 
void SourceReader::rewind_input() {
	source.unget();
}
	

