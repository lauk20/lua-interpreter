#include <memory>
#include <string>

#include "Expr.hpp"

using std::shared_ptr;
typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

class Interpreter : public ExprVisitor, public std::enable_shared_from_this<Interpreter> {
    variantX result;

    std::string stringify() {
        if (std::holds_alternative<double>(result)) {
            return std::to_string(std::get<double>(result));
        }

        if (std::holds_alternative<std::string>(result)) {
            return std::get<std::string>(result);
        }

        if (std::holds_alternative<bool>(result)) {
            if (std::get<bool>(result)) {
                return "true";
            }
            return "false";
        }

        if (std::holds_alternative<nullptr_t>(result)) {
            return "nil";
        }

        return "undefined";
    }

    bool isTruthy(variantX object) {
        if (object.index() == 3) return false;
        if (object.index() == 2) return std::get<2>(object);
        return true;
    }

    bool isEqual(variantX left, variantX right) {
        if (left.index() == 3 && right.index() == 3) return true;
        if (left.index() == 3 || right.index() == 3) return false;
        return left == right;
    }

    public:
        void visitLiteralExpr(shared_ptr<Literal> expr) {
            result = expr->value;
        }

        void visitUnaryExpr(shared_ptr<Unary> expr) {
            evaluate(expr->right); // this will set result to the right operand

            switch (expr->op.type) {
                case MINUS:
                    result = -(std::get<0>(result));
                case TILDE:
                    result = !(isTruthy(result));
            }
        }

        void visitGroupingExpr(shared_ptr<Grouping> expr) {
            evaluate(expr->expression);
        }

        void evaluate(shared_ptr<Expr> expr) {
            expr->accept(this->shared_from_this());
        }

        void visitBinaryExpr(shared_ptr<Binary> expr) {
            evaluate(expr->left);
            variantX left = result;
            evaluate(expr->right);
            variantX right = result;

            std::cout << "op type" << expr->op.type << std::endl;

            switch (expr->op.type) {
                case MINUS:
                    result = (double) (std::get<double>(left) - std::get<double>(right));
                    break;
                case SLASH:
                    result = (double) (std::get<double>(left) / std::get<double>(right));
                    break;
                case STAR:
                    result = (double) (std::get<double>(left) * std::get<double>(right));
                    break;
                case PLUS:
                    result = (double) (std::get<double>(left) + std::get<double>(right));
                    break;
                case GREATER:
                    std::cout << "here greater" << std::endl;
                    result = (bool) (std::get<double>(left) > std::get<double>(right));
                    break;
                case GREATER_EQUAL:
                    result = (bool) (std::get<double>(left) >= std::get<double>(right));
                    break;
                case LESS:
                    result = (bool) (std::get<double>(left) < std::get<double>(right));
                    break;
                case LESS_EQUAL:
                    result = (bool) (std::get<double>(left) <= std::get<double>(right));
                    break;
                case TILDE_EQUAL:
                    result = (bool) (!(isEqual(left, right)));
                    break;
                case EQUAL_EQUAL:
                    result = (bool) (isEqual(left, right));
                    break;
            }
        }

        void interpret(shared_ptr<Expr> expression) {
            /*
            try {
                evaluate(expression);
                std::cout << stringify(result) << std::endl;

            } catch (...) {
                std::cout << "err" << std::endl;
            }
            */
            evaluate(expression);
            std::cout << stringify() << std::endl;
        }
};