#include <string>
#include <vector>

#include "Token.cpp"

class Scanner {
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
                    addToken(MINUS);
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
            }
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

        char advance() {
            return source[current++];
        }

        void addToken(TokenType type) {
            addToken(type, NULL);
        }

        void addToken(TokenType type, std::any literal) {
            std::string text = source.substr(start, current - start + 1);
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
        }
};