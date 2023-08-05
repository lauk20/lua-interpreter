#include <any>
#include <iostream>
#include <string>
#include <memory>

#include "Token.hpp"
#include "LuaCallable_Forward.hpp"
typedef std::variant<double, std::string, bool, std::nullptr_t, std::shared_ptr<LuaCallable>> variantX;

Token::Token(TokenType type, std::string lexeme, variantX literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    return os << token.type << " " << token.lexeme << " " << token.line << std::endl;
}