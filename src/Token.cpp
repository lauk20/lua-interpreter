#include <any>
#include <iostream>
#include <string>

#include "TokenType.cpp"

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

        std::ostream& operator<<(std::ostream &os) {
            return os << type << " " << lexeme << std::endl;
        }
};