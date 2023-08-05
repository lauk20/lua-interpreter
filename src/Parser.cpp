#include <memory>
#include <vector>

#include "Parser.hpp"

using std::shared_ptr;
using std::make_shared;

shared_ptr<Expr> Parser::expression() {
    auto e = assignment();
    //std::cout << "we are back" << std::endl;
    return e;
}

/*
shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({LOCAL})) return varDeclaration();

        return statement();
    } catch (ParseError error) {
        synchronize();
        return nullptr;
    }
}
*/

shared_ptr<Stmt> Parser::statement() {
    try {
        if (match({FOR})) return forStatement();
        if (match({IF})) return ifStatement();
        if (match({WHILE})) return whileStatement();
        if (match({LOCAL})) return varDeclaration();
        if (match({FUNCTION})) return function();
        return expressionStatement();
    } catch (ParseError error) {
        synchronize();
        return nullptr;
    }
}

shared_ptr<Stmt> Parser::forStatement() {
    shared_ptr<Expr> initializer = assignment();
    Token name = std::static_pointer_cast<Variable>(initializer)->name;
    shared_ptr<Expression> init = make_shared<Expression>(initializer);

    consume(COMMA, "Expected ',' after initializer.");

    shared_ptr<Expr> end = nullptr;
    end = expression();
    //end = make_shared<Binary>(make_shared<Variable>(std::static_pointer_cast<Variable>(initializer)->name), Token(TILDE_EQUAL, "~=", nullptr, -1), end);

    /*
        This block of code is to ensure that the loop stops appropriately.
        the loop condition is where let n = the initialized value (begin number), i be the variable initialized, and e be the end value (inclusive end number);
        the loop continues where ( (n <= e && i <= e) || (n >= e && i >= e) )
        this is needed because Lua loops allow numeric loops by default and this is their continuation condition.
        we cannot just do <= or >= or ~= because there can be a loop like : for i = 10, 1, -2
        if we chose <=, the loop will never run, if we chose >= then it will be correct, but if we chose ~= it will not stop (infinite loop)
        same goes for a loop like: for i = 1, 10, 2
        2 of the 3 comparisions will not work
    */
    shared_ptr<Expr> left_left = make_shared<Binary>((std::static_pointer_cast<Assign>(initializer)->value), Token(LESS_EQUAL, "<=", nullptr, -1), end);
    shared_ptr<Expr> left_right = make_shared<Binary>(make_shared<Variable>(std::static_pointer_cast<Variable>(initializer)->name), Token(LESS_EQUAL, "<=", nullptr, -1), end);
    shared_ptr<Expr> right_left = make_shared<Binary>((std::static_pointer_cast<Assign>(initializer)->value), Token(GREATER_EQUAL, ">=", nullptr, -1), end);
    shared_ptr<Expr> right_right = make_shared<Binary>(make_shared<Variable>(std::static_pointer_cast<Variable>(initializer)->name), Token(GREATER_EQUAL, ">=", nullptr, -1), end);
    shared_ptr<Expr> left = make_shared<Logical>(left_left, Token(AND, "and", nullptr, -1), left_right);
    shared_ptr<Expr> right = make_shared<Logical>(right_left, Token(AND, "and", nullptr, -1), right_right);
    end = make_shared<Logical>(left, Token(OR, "or", nullptr, -1), right);

    shared_ptr<Expr> increment = nullptr;
    if (match({COMMA})) {
        shared_ptr<Expr> inc = expression();
        shared_ptr<Expr> incrementNode = make_shared<Binary>(make_shared<Variable>(std::static_pointer_cast<Variable>(initializer)->name), Token(PLUS, "+", nullptr, -1), inc);
        increment = make_shared<Variable>(name)->make_assignment(name, incrementNode, Token(PLUS, "", nullptr, -1), "Error in for loop increment.");
    } else { // Lua default increment is +1
        shared_ptr<Expr> incrementNode = make_shared<Binary>(make_shared<Variable>(std::static_pointer_cast<Variable>(initializer)->name), Token(PLUS, "+", nullptr, -1), make_shared<Literal>(1.0));
        increment = make_shared<Variable>(name)->make_assignment(name, incrementNode, Token(PLUS, "", nullptr, -1), "Error in for loop increment.");
    }

    consume(DO, "Expected 'do' after for initializer.");

    shared_ptr<Stmt> body = block();

    consume(END, "Expected 'end' to close for loop block.");

    // add incrementor to body and create while loop node
    {
        std::vector<shared_ptr<Stmt>> stmts;
        stmts.push_back(body);
        stmts.push_back(make_shared<Expression>(increment));
        body = make_shared<Block>(stmts);

        body = make_shared<While>(end, body);
    }

    // add initializer and while loop to body, creating block
    if (initializer != nullptr) {
        std::vector<shared_ptr<Stmt>> stmts;
        stmts.push_back(init);
        stmts.push_back(body);
        body = make_shared<Block>(stmts);
    }

    return body;
}

shared_ptr<Stmt> Parser::whileStatement() {
    shared_ptr<Expr> condition = expression();
    consume(DO, "Expected 'do' after while condition.");

    shared_ptr<Stmt> body = block();

    consume(END, "Expected 'end' after while block.");

    return make_shared<While>(condition, body);
}

shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    shared_ptr<Expr> initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }

    return make_shared<Var>(name, initializer);
}

shared_ptr<Stmt> Parser::elseifStatement() {
    shared_ptr<Expr> condition = expression();
    consume(THEN, "Expect 'then' after if condition.");

    shared_ptr<Stmt> thenBranch = block();
    std::vector<shared_ptr<Stmt>> elseifBranches;
    shared_ptr<Stmt> elseBranch = nullptr;

    return make_shared<If>(condition, thenBranch, elseifBranches, elseBranch);
}

shared_ptr<Stmt> Parser::ifStatement() {
    shared_ptr<Expr> condition = expression();
    consume(THEN, "Expect 'then' after if condition.");

    shared_ptr<Stmt> thenBranch = block();
    std::vector<shared_ptr<Stmt>> elseifBranches;
    while (match({ELSEIF}) && !isAtEnd()) {
        elseifBranches.push_back(elseifStatement());
    }
    shared_ptr<Stmt> elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = block();
    }

    consume(END, "Expect 'end' after conditional block.");

    return make_shared<If>(condition, thenBranch, elseifBranches, elseBranch);
}

shared_ptr<Stmt> Parser::expressionStatement() {
    shared_ptr<Expr> expr = expression();
    return make_shared<Expression>(expr);
}

shared_ptr<Function> Parser::function() {
    Token name = consume(IDENTIFIER, "Expect function name.");
    consume(LEFT_PAREN, "Expect '(' after function name.");

    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            parameters.push_back(consume(IDENTIFIER, "Expect parameter name"));
        } while (match({COMMA}));
    }

    consume(RIGHT_PAREN, "Expected ')' after parameters");

    shared_ptr<Stmt> body = block();

    consume(END, "Expected end after block.");

    return make_shared<Function>(name, parameters, body);
}

shared_ptr<Stmt> Parser::block() {
    std::vector<shared_ptr<Stmt>> statements;

    while (!check(RETURN) && !check(END) && !check(ELSEIF) && !check(ELSE) && !isAtEnd()) {
        statements.push_back(statement());
    }

    return make_shared<Block>(statements);
}

shared_ptr<Expr> Parser::assignment() {
    shared_ptr<Expr> expr = orParse();

    if (match({EQUAL})) {
        shared_ptr<Expr> value = assignment();

        return expr->make_assignment(std::static_pointer_cast<Variable>(expr)->name, value, previous(), "Invalid assignment target.");
    }

    return expr;
}

shared_ptr<Expr> Parser::orParse() {
    shared_ptr<Expr> expr = andParse();

    while (match({OR})) {
        Token op = previous();
        shared_ptr<Expr> right = andParse();
        expr = make_shared<Logical>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::andParse() {
    shared_ptr<Expr> expr = equality();

    while (match({AND})) {
        Token op = previous();
        shared_ptr<Expr> right = equality();
        expr = make_shared<Logical>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::equality() {
    //std::cout << "EQUALITY" << std::endl;
    auto expr = comparison();
    
    while (match({TILDE_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    //std::cout << "COMPARISON" << std::endl;
    auto expr = term();

    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::term() {
    //std::cout << "TERM" << std::endl;
    auto expr = factor();

    while (match({MINUS, PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::factor() {
    //std::cout << "FACTOR" << std::endl;
    auto expr = unary();

    while (match({SLASH, STAR})) {
        Token op = previous();
        auto right = unary();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    //std::cout << "UNARY" << std::endl;
    if (match({TILDE, MINUS})) {
        Token op = previous();
        auto right = unary();
        return make_shared<Unary>(op, right);
    }

    return call();
}

shared_ptr<Expr> Parser::finishCall(shared_ptr<Expr> callee) {
    std::vector<shared_ptr<Expr>> arguments;
    
    if (!check(RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match({COMMA}));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return make_shared<Call>(callee, paren, arguments);
}

shared_ptr<Expr> Parser::call() {
    shared_ptr<Expr> expr = primary();

    while (1) {
        if (match({LEFT_PAREN})) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

shared_ptr<Expr> Parser::primary() {
    //std::cout << "PRIMARY" << std::endl;
    if (match({FALSE})) return make_shared<Literal>(false);
    if (match({TRUE})) return make_shared<Literal>(true);
    if (match({NIL})) return make_shared<Literal>(nullptr);

    if (match({NUMBER, STRING})) {
        return make_shared<Literal>(LiteralVal(previous().literal));
    }

    if (match({IDENTIFIER})) {
        return make_shared<Variable>(previous());
    }

    if (match({LEFT_PAREN})) {
        auto expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return make_shared<Grouping>(expr);
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::match(std::vector<int> types) {
    for (int type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

Token Parser::consume(int type, std::string message) {
    if (check(type)) {
        auto e = advance();
        return e;
    }

    throw error(peek(), message);
}

bool Parser::check(int type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        current++; 
    }
    
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOFILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

ParseError Parser::error(Token token, std::string message) {
    Lua::error(token, message);
    return ParseError();
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch(peek().type) {
            case FUNCTION:
            case FOR:
            case IF:
            case WHILE:
            case RETURN:
                return;
        }

        advance();
    }
}

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

std::vector<shared_ptr<Stmt>> Parser::parse() {
    std::vector<shared_ptr<Stmt>> statements = std::static_pointer_cast<Block>(block())->statements;

    return statements;
}
