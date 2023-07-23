#include <string>
#include <vector>
#include<unordered_map>

#include "Token.hpp"

typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

/// Scanner class to read source code into tokens
class Scanner {
    /// @brief map of reserved identifiers
    static const std::unordered_map<std::string, TokenType> keywords;
    /// @brief  source code string
    std::string source;
    /// @brief vector of tokens read from the source code
    std::vector<Token> tokens;
    /// @brief index of the start of the current token in the source code string
    int start = 0;
    /// @brief index of the current position in the source code string
    int current = 0;
    /// @brief  line number of the current position
    int line = 1;

    private:
        /// @brief determine whether the current position is the end of the source code
        /// @return true if is end of source code string, false otherwise
        bool isAtEnd();

        /// @brief function to scan the source code for the next token
        void scanToken();

        /// @brief read identifiers
        void identifier();

        /// @brief read numbers as long doubles, includes decimal numbers
        void number();

        /// @brief read string literals between " and ".
        void string();

        /// @brief determine whether the next token matches the expected token.
        /// consumes it if it does and increments the current position
        /// @param expected the expected token to match
        /// @return true if the next token matches the expected, false otherwise
        bool match(char expected);

        /// @brief get the character at the current position
        /// @return the character at the current position of the source code. null char if at end
        char peek();

        /// @brief get the character after the current position
        /// @return the character at the position after current position of source code. null if is the end
        char peekNext();

        /// @brief determine whether a char is an alphabet
        /// @param c the char to determine
        /// @return true if is an alphabet, false otherwise
        bool isAlpha(char c);

        /// @brief determine whether a char is an alphabet or digit
        /// @param c the char to determine
        /// @return true if is an alphabet or digit, false otherwise
        bool isAlphaNumeric(char c);

        /// @brief determine whether c is a digit
        /// @param c char to determine
        /// @return true if c is a digit, false otherwise
        bool isDigit(char c);

        /// @brief consume the current character and then increments the current index
        /// @return the character at the current index
        char advance();

        /// @brief add token to the token vector
        /// @param type the TokenType to add
        void addToken(TokenType type);

        /// @brief add token to the token vector
        /// @param type the TokenType to add
        /// @param literal the literal value of the token
        void addToken(TokenType type, variantX literal);

    public:
        /// @brief constructor for Scanner class
        /// @param source the source code as a string
        Scanner(std::string source);

        /// @brief scan for all tokens
        /// @return the vector of tokens
        std::vector<Token> scanTokens();
};