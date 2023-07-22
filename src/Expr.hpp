#ifndef Expr_H
#define Expr_H

#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "Token.hpp"

typedef std::variant<double, std::string> variantX;

class Expr;
class ExprVisitor;
class Binary;
class Grouping;
class Literal;
class Unary;

class Expr {

    public:
        virtual void accept(std::shared_ptr<ExprVisitor> visitor) = 0;

};

class ExprVisitor {
    public:
        virtual void visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual void visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual void visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual void visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;

        Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

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
        variantX value;

        Literal(variantX value);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};


class Unary : public Expr, public std::enable_shared_from_this<Unary> {
    public:
        Token op;
        std::shared_ptr<Expr> right;

        Unary(Token op, std::shared_ptr<Expr> right);

        void accept(std::shared_ptr<ExprVisitor> visitor);
};

#endif