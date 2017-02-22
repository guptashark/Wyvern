CC = cl
SRC =  main.cpp 

all:
	$(CC) /EHsc $(SRC)
	main.exe

