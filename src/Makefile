CC = g++

CFLAGS = -std=c++17

all: Lua.o Scanner.o Token.o
	$(CC) $(CFLAGS) -o Lua Lua.o Scanner.o Token.o

Lua.o: Lua.cpp Lua.hpp Scanner.hpp
	$(CC) $(CFLAGS) -c Lua.cpp

Scanner.o: Scanner.hpp Token.hpp
	$(CC) $(CFLAGS) -c Scanner.cpp

Token.o: Token.hpp
	$(CC) $(CFLAGS) -c Token.cpp