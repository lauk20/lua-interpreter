#include <memory>
#include <vector>

#include "Parser.hpp"
#include "Expr.hpp"

using std::shared_ptr;
using std::make_shared;

shared_ptr<Expr> Parser::expression() {
    return equality();
}

shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    
    while (match({TILDE_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (match({MINUS, PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while (match({SLASH, STAR})) {
        Token op = previous();
        auto right = unary();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    if (match({TILDE, MINUS})) {
        Token op = previous();
        auto right = unary();
        return make_shared<Unary>(op, right);
    }

    return primary();
}

shared_ptr<Expr> Parser::primary() {
    if (match({FALSE})) return make_shared<Literal>(false);
    if (match({TRUE})) return make_shared<Literal>(true);
    if (match({NIL})) return make_shared<Literal>(0);

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
        return advance();
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

shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return NULL;
    }
}
