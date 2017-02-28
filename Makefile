CC = cl
SRC =  main.cpp lexer.cpp parser.cpp

all:
	$(CC) /EHsc $(SRC)
	main.exe

