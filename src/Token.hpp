#include <any>
#include <iostream>
#include <string>

#include "TokenType.cpp"

/// Class to represent a Token
class Token {
    public:
        /// @brief Type of Token
        TokenType type;
        /// @brief The Token
        std::string lexeme;
        /// @brief literal value if applicable
        std::any literal;
        /// @brief the line where the token exists
        int line;

        /// @brief Token constructor
        /// @param type the token type
        /// @param lexeme the token string
        /// @param literal literal value of the token
        /// @param line line where the token exists
        Token(TokenType type, std::string lexeme, std::any literal, int line);

        /// @brief overloaded operator << for printing
        /// @param os the output stream
        /// @param token the token
        /// @return the provided output stream (os)
        friend std::ostream& operator<<(std::ostream &os, const Token& token);
};