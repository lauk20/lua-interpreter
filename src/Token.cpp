#include <any>
#include <iostream>
#include <string>

#include "Token.hpp"

class Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    public:
        Token(TokenType type, std::string lexeme, std::any literal, int line) {
            this->type = type;
            this->lexeme = lexeme;
            this->literal = literal;
            this->line = line;
        }

        friend std::ostream& operator<<(std::ostream &os, const Token& token);
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
    return os << token.type << " " << token.lexeme << " " << token.line << std::endl;
}