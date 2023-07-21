#include <any>
#include <vector>
#include <memory>
#include <variant>

#include "Token.hpp"

using std::shared_ptr;
typedef std::variant<double, std::string> variantX;

template <class T>
class Binary;

template <class T>
class Grouping;

template <class T>
class Literal;

template <class T>
class Unary;

template <class T>
class ExprVisitor;

template <class T>
class Expr {

    public:
        virtual T accept(shared_ptr<ExprVisitor<T>> visitor) = 0;

};

template <class T>
class ExprVisitor {
    public:
        virtual T visitBinaryExpr(shared_ptr<Binary<T>> expr) = 0;
        virtual T visitGroupingExpr(shared_ptr<Grouping<T>> expr) = 0;
        virtual T visitLiteralExpr(shared_ptr<Literal<T>> expr) = 0;
        virtual T visitUnaryExpr(shared_ptr<Unary<T>> expr) = 0;
};

template <class T>
class Binary : public Expr<T>, public std::enable_shared_from_this<Binary<T>> {
    public:
        shared_ptr<Expr<T>> left;
        Token op;
        shared_ptr<Expr<T>> right;

        Binary(shared_ptr<Expr<T>> left, Token op, shared_ptr<Expr<T>> right) : op(op) {
            this->left = left;
            this->op = op;
            this->right = right;
        }

        T accept(shared_ptr<ExprVisitor<T>> visitor) {
            std::cout << "BIN" << std::endl;
            return visitor->visitBinaryExpr(this->shared_from_this());
        }
};

template <class T>
class Grouping : public Expr<T>, public std::enable_shared_from_this<Grouping<T>> {
    public:
        shared_ptr<Expr<T>> expression;

        Grouping(shared_ptr<Expr<T>> expression) {
            this->expression = expression;
        }

        T accept(shared_ptr<ExprVisitor<T>> visitor) {
            return visitor->visitGroupingExpr(this->shared_from_this());
        }
};

template <class T>
class Literal : public Expr<T>, public std::enable_shared_from_this<Literal<T>> {
    public:
        variantX value;

        Literal(variantX value) {
            this->value = value;
        }

        T accept(shared_ptr<ExprVisitor<T>> visitor) {
            return visitor->visitLiteralExpr(this->shared_from_this());
        }
};

template <class T>
class Unary : public Expr<T>, public std::enable_shared_from_this<Unary<T>> {
    public:
        Token op;
        shared_ptr<Expr<T>> right;

        Unary(Token op, shared_ptr<Expr<T>> right) : op(op) {
            this->op = op;
            this->right = right;
        }

        T accept(shared_ptr<ExprVisitor<T>> visitor) {
            return visitor->visitUnaryExpr(this->shared_from_this());
        }
};

