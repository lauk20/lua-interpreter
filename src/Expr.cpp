#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "Token.hpp"

using std::shared_ptr;
typedef std::variant<double, std::string> variantX;

class Binary;

class Grouping;

class Literal;

class Unary;

class ExprVisitor;

class Expr {

    public:
        virtual void accept(shared_ptr<ExprVisitor> visitor) = 0;

};

class ExprVisitor {
    public:
        virtual void visitBinaryExpr(shared_ptr<Binary> expr) = 0;
        virtual void visitGroupingExpr(shared_ptr<Grouping> expr) = 0;
        virtual void visitLiteralExpr(shared_ptr<Literal> expr) = 0;
        virtual void visitUnaryExpr(shared_ptr<Unary> expr) = 0;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
    public:
        shared_ptr<Expr> left;
        Token op;
        shared_ptr<Expr> right;

        Binary(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right) : op(op) {
            this->left = left;
            this->op = op;
            this->right = right;
        }

        void accept(shared_ptr<ExprVisitor> visitor) {
            std::cout << "BIN" << std::endl;
            return visitor->visitBinaryExpr(this->shared_from_this());
        }
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
    public:
        shared_ptr<Expr> expression;

        Grouping(shared_ptr<Expr> expression) {
            this->expression = expression;
        }

        void accept(shared_ptr<ExprVisitor> visitor) {
            return visitor->visitGroupingExpr(this->shared_from_this());
        }
};


class Literal : public Expr, public std::enable_shared_from_this<Literal> {
    public:
        variantX value;

        Literal(variantX value) {
            this->value = value;
        }

        void accept(shared_ptr<ExprVisitor> visitor) {
            return visitor->visitLiteralExpr(this->shared_from_this());
        }
};


class Unary : public Expr, public std::enable_shared_from_this<Unary> {
    public:
        Token op;
        shared_ptr<Expr> right;

        Unary(Token op, shared_ptr<Expr> right) : op(op) {
            this->op = op;
            this->right = right;
        }

        void accept(shared_ptr<ExprVisitor> visitor) {
            return visitor->visitUnaryExpr(this->shared_from_this());
        }
};

