#ifndef __SOURCE_READER_H__
#define __SOURCE_READER_H__

#include <fstream>
#include <string>

class SourceReader { 

	private:
		std::fstream source;


	public:
		SourceReader(std::string src_file);
		char next_char();
};

#endif
