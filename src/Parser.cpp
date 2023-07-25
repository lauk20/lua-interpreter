#include <memory>
#include <vector>

#include "Parser.hpp"

using std::shared_ptr;
using std::make_shared;

shared_ptr<Expr> Parser::expression() {
    auto e = equality();
    //std::cout << "we are back" << std::endl;
    return e;
}

shared_ptr<Stmt> Parser::statement() {
    return expressionStatement();
}

shared_ptr<Stmt> Parser::expressionStatement() {
    shared_ptr<Expr> expr = expression();
    return make_shared<Expression>(expr);
}

shared_ptr<Expr> Parser::equality() {
    //std::cout << "EQUALITY" << std::endl;
    auto expr = comparison();
    
    while (match({TILDE_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    //std::cout << "COMPARISON" << std::endl;
    auto expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::term() {
    //std::cout << "TERM" << std::endl;
    auto expr = factor();

    while (match({MINUS, PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::factor() {
    //std::cout << "FACTOR" << std::endl;
    auto expr = unary();

    while (match({SLASH, STAR})) {
        Token op = previous();
        auto right = unary();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    //std::cout << "UNARY" << std::endl;
    if (match({TILDE, MINUS})) {
        Token op = previous();
        auto right = unary();
        return make_shared<Unary>(op, right);
    }

    return primary();
}

shared_ptr<Expr> Parser::primary() {
    //std::cout << "PRIMARY" << std::endl;
    if (match({FALSE})) return make_shared<Literal>(false);
    if (match({TRUE})) return make_shared<Literal>(true);
    if (match({NIL})) return make_shared<Literal>(nullptr);

    if (match({NUMBER, STRING})) {
        return make_shared<Literal>(previous().literal);
    }

    if (match({LEFT_PAREN})) {
        auto expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return make_shared<Grouping>(expr);
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::match(std::vector<int> types) {
    for (int type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

Token Parser::consume(int type, std::string message) {
    if (check(type)) {
        auto e = advance();
        return e;
    }

    throw error(peek(), message);
}

bool Parser::check(int type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        current++; 
    }
    
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOFILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

ParseError Parser::error(Token token, std::string message) {
    Lua::error(token, message);
    return ParseError();
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch(peek().type) {
            case FUNCTION:
            case FOR:
            case IF:
            case WHILE:
            case RETURN:
                return;
        }

        advance();
    }
}

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

std::vector<shared_ptr<Stmt>> Parser::parse() {
    std::vector<shared_ptr<Stmt>> statements;

    while (!isAtEnd()) {
        statements.push_back(statement());
    }

    return statements;
}
