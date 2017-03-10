CC = cl
SRC =  main.cpp lexer.cpp parser.cpp
CPPFLAGS = /c /EHsc /nologo
LINKFLAGS = /nologo

#all: 
#	$(CC) /EHsc $(SRC)

all: main.obj lexer.obj parser.obj
	LINK $(LINKFLAGS) main.obj lexer.obj parser.obj
	main.exe

main.obj: main.cpp
	$(CC) $(CPPFLAGS) main.cpp

lexer.obj: lexer.cpp
	$(CC) $(CPPFLAGS) lexer.cpp

parser.obj: parser.cpp
	$(CC) $(CPPFLAGS) parser.cpp

clean:
	del main.obj
	del lexer.obj
	del parser.obj
	del main.exe


