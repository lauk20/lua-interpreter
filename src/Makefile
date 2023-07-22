CC = g++

CFLAGS = -std=c++17

all: Lua.o Scanner.o Parser.o ParseError.o AstPrinter.o Expr.o
	$(CC) $(CFLAGS) -o Lua Lua.o Scanner.o Token.o Parser.o ParseError.o AstPrinter.o Expr.o

Lua.o: Lua.cpp Lua.hpp Scanner.hpp Parser.hpp
	$(CC) $(CFLAGS) -c Lua.cpp

Scanner.o: Scanner.hpp Token.o
	$(CC) $(CFLAGS) -c Scanner.cpp

Token.o: Token.hpp
	$(CC) $(CFLAGS) -c Token.cpp

Parser.o: Parser.cpp Parser.hpp
	$(CC) $(CFLAGS) -c Parser.cpp

ParseError.o: ParseError.cpp
	$(CC) $(CFLAGS) -c ParseError.cpp

AstPrinter.o: AstPrinter.cpp AstPrinter.hpp Expr.hpp
	$(CC) $(CFLAGS) -c AstPrinter.cpp

Expr.o: Expr.cpp Expr.hpp
	$(CC) $(CFLAGS) -c Expr.cpp