#ifndef Expr_H
#define Expr_H

#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "Literal.hpp"
#include "ParseError.hpp"
#include "Token.hpp"
#include "LuaCallable_Forward.hpp"

class Expr;
class ExprVisitor;
class Assign;
class Binary;
class Call;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

class Expr {

    public:
        virtual void accept(std::shared_ptr<ExprVisitor> visitor) = 0;
        virtual std::shared_ptr<Expr> make_assignment(Token name, std::shared_ptr<Expr> value, Token equal, std::string errMessage);
};

class ExprVisitor {
    public:
        virtual void visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
        virtual void visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual void visitCallExpr(std::shared_ptr<Call> expr) = 0;
        virtual void visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual void visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual void visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
        virtual void visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual void visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
};

class Assign : public Expr, public std::enable_shared_from_this<Assign> {
    public:
        Token name;
        std::shared_ptr<Expr> value;

        Assign(Token name, std::shared_ptr<Expr> value);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;

        Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

class Call : public Expr, public std::enable_shared_from_this<Call> {
    public:
        std::shared_ptr<Expr> callee;
        Token paren;
        std::vector<std::shared_ptr<Expr>> arguments;

        Call(std::shared_ptr<Expr> callee, Token parent, std::vector<std::shared_ptr<Expr>> arguments);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
    public:
        std::shared_ptr<Expr> expression;

        Grouping(std::shared_ptr<Expr> expression);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};


class Literal : public Expr, public std::enable_shared_from_this<Literal> {
    public:
        LiteralVal value;

        Literal(LiteralVal value);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

class Logical : public Expr, public std::enable_shared_from_this<Logical> {
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        
        Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};


class Unary : public Expr, public std::enable_shared_from_this<Unary> {
    public:
        Token op;
        std::shared_ptr<Expr> right;

        Unary(Token op, std::shared_ptr<Expr> right);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

class Variable : public Expr, public std::enable_shared_from_this<Variable> {
    public:
        Token name;

        Variable(Token name);

        void accept(std::shared_ptr<ExprVisitor> visitor);

        std::shared_ptr<Expr> make_assignment(Token name, std::shared_ptr<Expr> value, Token equal, std::string errMessage);
};

#endif