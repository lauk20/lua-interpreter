#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "Expr.hpp"
#include "Token.hpp"

using std::shared_ptr;

Binary::Binary(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right) : op(op) {
    this->left = left;
    this->op = op;
    this->right = right;
}

void Binary::accept(shared_ptr<ExprVisitor> visitor) {
    std::cout << "BIN" << std::endl;
    return visitor->visitBinaryExpr(this->shared_from_this());
}

Grouping::Grouping(shared_ptr<Expr> expression) {
    this->expression = expression;
}

void Grouping::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitGroupingExpr(this->shared_from_this());
}


Literal::Literal(variantX value) {
    this->value = value;
}

void Literal::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitLiteralExpr(this->shared_from_this());
}


Unary::Unary(Token op, shared_ptr<Expr> right) : op(op) {
    this->op = op;
    this->right = right;
}

void Unary::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitUnaryExpr(this->shared_from_this());
}