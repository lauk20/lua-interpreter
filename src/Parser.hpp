#include <memory>
#include <vector>

#include "Lua.hpp"
#include "ParseError.cpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"

using std::shared_ptr;

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    shared_ptr<Expr> expression();

    shared_ptr<Stmt> statement();

    shared_ptr<Stmt> expressionStatement();

    shared_ptr<Expr> equality();

    shared_ptr<Expr> comparison();

    shared_ptr<Expr> term();

    shared_ptr<Expr> factor();

    shared_ptr<Expr> unary();

    shared_ptr<Expr> primary();

    bool match(std::vector<int> types);

    Token consume(int type, std::string message);

    bool check(int type);

    Token advance();

    bool isAtEnd();

    Token peek();

    Token previous();

    ParseError error(Token token, std::string message);

    void synchronize(); 

    public:
        Parser(std::vector<Token> tokens);
        std::vector<shared_ptr<Stmt>> parse();
};