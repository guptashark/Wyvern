CC = cl
SRC =  main.cpp lexer.cpp parser.cpp
CPPFLAGS = /c /EHsc /nologo /Iinclude /Foobj/
LINKFLAGS = /nologo

#all: 
#	cd src 
#	nmake 
#	cd ../obj
#	nmake

all: bin/wyvern.exe

bin/wyvern.exe: obj/main.obj "obj/lexer.obj" "obj/parser.obj" "obj/deterministicfa.obj" "obj/nondeterministicfa.obj" "obj/parsetree.obj"
	LINK /OUT:bin/wyvern.exe $(LINKFLAGS) $** 

obj/main.obj: src/main.cpp
	$(CC) $(CPPFLAGS) $** 

obj/lexer.obj: src/lexer.cpp
	$(CC) $(CPPFLAGS) $**

obj/parser.obj: src/parser.cpp
	$(CC) $(CPPFLAGS) $**

obj/parsetree.obj: "src/parsetree.cpp"
	$(CC) $(CPPFLAGS) $**

obj/nondeterministicfa.obj: "src/nondeterministicfa.cpp"
	$(CC) $(CPPFLAGS) $**

obj/deterministicfa.obj: "src/deterministicfa.cpp"
	$(CC) $(CPPFLAGS) $**

clean:
	del main.obj
	del lexer.obj
	del parser.obj
	del main.exe


