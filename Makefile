CC = cl
SRC =  main.cpp lexer.cpp parser.cpp
CPPFLAGS = /c /EHsc /nologo /Iinclude /Foobj/ /W3
LINKFLAGS = /nologo

all: bin/wyvern.exe


bin/wyvern.exe: obj/main.obj "obj/lexer.obj" "obj/parser.obj" "obj/deterministicfa.obj" "obj/hardcodenfa.obj" "obj/parsetree.obj" "obj/source_reader.obj" "obj/token.obj" "obj/symbol_table.obj"
	@LINK /OUT:bin/wyvern.exe $(LINKFLAGS) $** 

obj/main.obj: src/main.cpp
	@$(CC) $(CPPFLAGS) $** 

obj/lexer.obj: src/lexer.cpp
	@$(CC) $(CPPFLAGS) $**

obj/parser.obj: src/parser.cpp
	@$(CC) $(CPPFLAGS) $**

obj/parsetree.obj: "src/parsetree.cpp"
	@$(CC) $(CPPFLAGS) $**

obj/hardcodenfa.obj: "src/hardcodenfa.cpp"
	@$(CC) $(CPPFLAGS) $**

obj/deterministicfa.obj: "src/deterministicfa.cpp"
	@$(CC) $(CPPFLAGS) $**

obj/source_reader.obj: "src/source_reader.cpp"
	@$(CC) $(CPPFLAGS) $**

obj/token.obj: "src/token.cpp"
	@$(CC) $(CPPFLAGS) $**

obj/symbol_table.obj: "src/symbol_table.cpp"
	@$(CC) $(CPPFLAGS) $**

clean:
	del obj\main.obj
	del obj\lexer.obj
	del obj\parser.obj
	del obj\hardcodenfa.obj
	del obj\deterministicfa.obj
	del obj\parsetree.obj
	del obj\source_reader.obj
	del obj\token.obj
	del obj\symbol_table.obj
	del bin\wyvern.exe


