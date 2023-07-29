#include <string>
#include <vector>
#include <unordered_map>

#include "Scanner.hpp"
#include "Lua.hpp"

bool Scanner::isAtEnd() {
    return current >= source.size();
}

void Scanner::scanToken() {
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
        case '/':
            addToken(SLASH);
            break;
        case '~':
            addToken(match('=') ? TILDE_EQUAL : TILDE);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
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

void Scanner::identifier() {
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

void Scanner::number() {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
    }

    while (isDigit(peek())) {
        advance();
    }

    addToken(NUMBER, (double) std::stold(source.substr(start, current - start).c_str()));
}

void Scanner::string() {
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

bool Scanner::match(char expected) {
    if (isAtEnd()) {
        return false;
    }
    if (source[current] != expected) {
        return false;
    }

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) {
        return '\0';
    }

    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) {
        return '\0';
    }

    return source[current + 1];
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'z') ||
            (c == '_');
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::advance() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, variantX literal) {
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

Scanner::Scanner(std::string source) {
    this->source = source;
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(EOFILE, "", nullptr, line));

    return tokens;
}

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
    {"true", TRUE},
    {"until", UNTIL},
    {"while", WHILE}
};