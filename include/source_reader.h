#ifndef __SOURCE_READER_H__
#define __SOURCE_READER_H__

#include <fstream>
#include <string>

class SourceReader { 

	private:
		bool from_shell;
		std::ifstream source;

	public:
		SourceReader();
		SourceReader(std::string src_file);
		char next_char();
		void rewind_input();
};

#endif
