#include <iostream>
#include "source_reader.h"

using namespace std;

SourceReader::SourceReader(string src_file): stored_str("fora") {

	cout << "Constructor called with src file: ";
	cout << src_file;
	cout << endl;
	it = stored_str.begin();
}

char SourceReader::next_char() {
	if(it == stored_str.end()) {
		return EOF;
	}

	char ret = *it;
	it++;
	return ret;
}
	

