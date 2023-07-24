#include <memory>
#include <string>
#include <stdexcept>

#include "Expr.hpp"
#include "Lua.hpp"
#include "RuntimeError.hpp"

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
                    checkNumberOperand(expr->op, result);
                    result = -(std::get<0>(result));
                case TILDE:
                    result = !(isTruthy(result));
            }
        }

        void checkNumberOperand(Token op, variantX operand) {
            if (std::holds_alternative<double>(operand)) return;
            throw RuntimeError(op, "Operand must be a number.");
        }

        void checkNumberOperands(Token op, variantX left, variantX right) {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return;
            }

            throw RuntimeError(op, "Operands must be numbers.");
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

            switch (expr->op.type) {
                case MINUS:
                    checkNumberOperands(expr->op, left, right);
                    result = (double) (std::get<double>(left) - std::get<double>(right));
                    break;
                case SLASH:
                    checkNumberOperands(expr->op, left, right);
                    result = (double) (std::get<double>(left) / std::get<double>(right));
                    break;
                case STAR:
                    checkNumberOperands(expr->op, left, right);
                    result = (double) (std::get<double>(left) * std::get<double>(right));
                    break;
                case PLUS:
                    checkNumberOperands(expr->op, left, right);
                    result = (double) (std::get<double>(left) + std::get<double>(right));
                    break;
                case GREATER:
                    checkNumberOperands(expr->op, left, right);
                    result = (bool) (std::get<double>(left) > std::get<double>(right));
                    break;
                case GREATER_EQUAL:
                    checkNumberOperands(expr->op, left, right);
                    result = (bool) (std::get<double>(left) >= std::get<double>(right));
                    break;
                case LESS:
                    checkNumberOperands(expr->op, left, right);
                    result = (bool) (std::get<double>(left) < std::get<double>(right));
                    break;
                case LESS_EQUAL:
                    checkNumberOperands(expr->op, left, right);
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
            try {
                evaluate(expression);
                std::cout << stringify() << std::endl;

            } catch (RuntimeError error) {
                Lua::runtimeError(error);
            }
        }
};