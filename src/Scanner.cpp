#include <string>
#include <vector>
#include <unordered_map>

#include "Scanner.hpp"

class Scanner {
    static const std::unordered_map<std::string, TokenType> keywords;
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    private:
        bool isAtEnd() {
            return current >= source.size();
        }

        void scanToken() {
            char c = advance();

            switch(c) {
                case '(':
                    addToken(LEFT_PAREN);
                    break;
                case ')':
                    addToken(RIGHT_PAREN);
                    break;
                case '{':
                    addToken(LEFT_BRACE);
                    break;
                case '}':
                    addToken(RIGHT_BRACE);
                    break;
                case ',':
                    addToken(COMMA);
                    break;
                case '.':
                    addToken(DOT);
                    break;
                case '-':
                    if (match('-')) {
                        while (peek() != '\n' && !isAtEnd()) {
                            advance();
                        }
                    } else {
                        addToken(MINUS);
                    }

                    break;
                case '+':
                    addToken(PLUS);
                    break;
                case ';':
                    addToken(SEMICOLON);
                    break;
                case '*':
                    addToken(STAR);
                    break;
                case '~':
                    addToken(match('=') ? TILDE_EQUAL : TILDE);
                    break;
                case '=':
                    addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                    break;
                case '<':
                    addToken(match('=') ? LESS_EQUAL : EQUAL);
                    break;
                case '>':
                    addToken(match('=') ? GREATER_EQUAL : EQUAL);
                    break;
                case ' ':
                    break;
                case '\r':
                    break;
                case '\t':
                    break;
                case '\n':
                    break;

                case '"':
                    string();
                    break;
                
                default:
                    if (isDigit(c)) {
                        number();
                    } else if (isAlpha(c)) {
                        identifier();
                    } else {
                        Lua::error(line, "Unexpected character.");
                    }
                    break;
            }
        }

        void identifier() {
            while (isAlphaNumeric(peek())) {
                advance();
            }

            std::string text = source.substr(start, current - start);
            TokenType type = IDENTIFIER;
            if (keywords.find(text) != keywords.end()) {
                type = keywords.at(text);
            }

            addToken(type);
        }

        void number() {
            while (isDigit(peek())) {
                advance();
            }

            if (peek() == '.' && isDigit(peekNext())) {
                advance();
            }

            while (isDigit(peek())) {
                advance();
            }

            addToken(NUMBER, std::stold(source.substr(start, current - start).c_str()));
        }

        void string() {
            while (peek() != '"' && !isAtEnd()) {
                if (peek() == '\n') line++;
                advance();
            }

            if (isAtEnd()) {
                Lua::error(line, "Unterminated string.");
                return;
            }

            advance();

            std::string value = source.substr(start, current - start);
            addToken(STRING, value);
        }

        bool match(char expected) {
            if (isAtEnd()) {
                return false;
            }
            if (source[current] != expected) {
                return false;
            }

            current++;
            return true;
        }

        char peek() {
            if (isAtEnd()) {
                return '\0';
            }

            return source[current];
        }

        char peekNext() {
            if (current + 1 >= source.size()) {
                return '\0';
            }

            return source[current + 1];
        }

        bool isAlpha(char c) {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'z') ||
                   (c == '_');
        }

        bool isAlphaNumeric(char c) {
            return isAlpha(c) || isDigit(c);
        }

        bool isDigit(char c) {
            return c >= '0' && c <= '9';
        }

        char advance() {
            return source[current++];
        }

        void addToken(TokenType type) {
            addToken(type, NULL);
        }

        void addToken(TokenType type, std::any literal) {
            std::string text = source.substr(start, current - start);
            tokens.push_back(Token(type, text, literal, line));
        }

    public:
        Scanner(std::string source) {
            this->source = source;
        }

        std::vector<Token> scanTokens() {
            while (!isAtEnd()) {
                start = current;
                scanToken();
            }

            return tokens;
        }
};

const std::unordered_map<std::string, TokenType> Scanner::keywords {
    {"and", AND},
    {"break", BREAK},
    {"do", DO},
    {"else", ELSE},
    {"elseif", ELSEIF},
    {"end", END},
    {"false", FALSE},
    {"for", FOR},
    {"function", FUNCTION},
    {"if", IF},
    {"in", IN},
    {"nil", NIL},
    {"not", NOT},
    {"or", OR},
    {"repeat", REPEAT},
    {"return", RETURN},
    {"then", THEN},
    {"until", UNTIL},
    {"while", WHILE}
};