#include <memory>
#include <vector>

#include "Lua.hpp"
#include "ParseError.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"

using std::shared_ptr;

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    shared_ptr<Expr> expression();

    shared_ptr<Stmt> declaration();

    shared_ptr<Stmt> statement();

    shared_ptr<Stmt> whileStatement();

    shared_ptr<Stmt> elseifStatement();

    shared_ptr<Stmt> ifStatement();

    shared_ptr<Stmt> expressionStatement();

    shared_ptr<Stmt> block();

    shared_ptr<Expr> assignment();

    shared_ptr<Expr> orParse();

    shared_ptr<Expr> andParse();

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