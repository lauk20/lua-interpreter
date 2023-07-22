#include <any>
#include <iostream>
#include <string>

#include "Token.hpp"

Token::Token(TokenType type, std::string lexeme, std::variant<double, std::string> literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    return os << token.type << " " << token.lexeme << " " << token.line << std::endl;
}