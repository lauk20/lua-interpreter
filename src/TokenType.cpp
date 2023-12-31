#ifndef TokenType_H
#define TokenType_H

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    TILDE, TILDE_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER, LOCAL,

    AND, BREAK, DO, ELSE, ELSEIF, END, FALSE, FUNCTION, FOR, IF, IN, NIL, NOT,
    OR, REPEAT, RETURN, THEN, TRUE, UNTIL, WHILE,

    EOFILE
};

#endif