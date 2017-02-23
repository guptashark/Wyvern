CC = cl
SRC =  main.cpp lexer.cpp

all:
	$(CC) /EHsc $(SRC)
	main.exe

