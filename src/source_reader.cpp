#include <iostream>
#include "source_reader.h"

using namespace std;

SourceReader::SourceReader(string src_file) {

	// open the source file. 
	source.open(src_file);	

}

char SourceReader::next_char() {
	return source.get();
}

void SourceReader::rewind_input() {
	source.unget();
}
	

