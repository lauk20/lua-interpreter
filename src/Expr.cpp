#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "ParseError.hpp"
#include "Expr.hpp"
#include "Lua.hpp"

using std::shared_ptr;

shared_ptr<Expr> Expr::make_assignment(Token varName, shared_ptr<Expr> value, Token equal, std::string errMessage) {
    Lua::error(equal, errMessage);
    throw ParseError();
    return std::make_shared<Assign>(varName, value);
}

Assign::Assign(Token name, shared_ptr<Expr> value) : name(name), value(value) {

}

void Assign::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitAssignExpr(this->shared_from_this());
}

Binary::Binary(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right) : op(op) {
    this->left = left;
    this->op = op;
    this->right = right;
}

void Binary::accept(shared_ptr<ExprVisitor> visitor) {
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

Logical::Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {

}

void Logical::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitLogicalExpr(this->shared_from_this());
}


Unary::Unary(Token op, shared_ptr<Expr> right) : op(op) {
    this->op = op;
    this->right = right;
}

void Unary::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitUnaryExpr(this->shared_from_this());
}

Variable::Variable(Token name) : name(name) {

}

void Variable::accept(shared_ptr<ExprVisitor> visitor) {
    return visitor->visitVariableExpr(this->shared_from_this());
}

shared_ptr<Expr> Variable::make_assignment(Token name, shared_ptr<Expr> value, Token equal, std::string errMessage) {
    return std::make_shared<Assign>(name, value);
}