#include <memory>
#include <string>
#include <stdexcept>

#include "Interpreter.hpp"

using std::shared_ptr;
typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;


Interpreter::Interpreter(shared_ptr<Environment> environment) : environment(environment) { }

std::string Interpreter::stringify() {
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

bool Interpreter::isTruthy(variantX object) {
    if (object.index() == 3) return false;
    if (object.index() == 2) return std::get<2>(object);
    return true;
}

bool Interpreter::isEqual(variantX left, variantX right) {
    if (left.index() == 3 && right.index() == 3) return true;
    if (left.index() == 3 || right.index() == 3) return false;
    return left == right;
}

void Interpreter::visitLiteralExpr(shared_ptr<Literal> expr) {
    result = expr->value;
}

void Interpreter::visitUnaryExpr(shared_ptr<Unary> expr) {
    evaluate(expr->right); // this will set result to the right operand

    switch (expr->op.type) {
        case MINUS:
            checkNumberOperand(expr->op, result);
            result = -(std::get<0>(result));
        case TILDE:
            result = !(isTruthy(result));
    }
}

void Interpreter::visitVariableExpr(shared_ptr<Variable> expr) {
    result = environment->get(expr->name);
}

void Interpreter::checkNumberOperand(Token op, variantX operand) {
    if (std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, variantX left, variantX right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        return;
    }

    throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::visitGroupingExpr(shared_ptr<Grouping> expr) {
    evaluate(expr->expression);
}

void Interpreter::evaluate(shared_ptr<Expr> expr) {
    expr->accept(this->shared_from_this());
}

void Interpreter::execute(shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}

void Interpreter::executeBlock(std::vector<shared_ptr<Stmt>> statements, shared_ptr<Environment> env) {
    auto pre = environment;

    try {
        environment = env;

        for (shared_ptr<Stmt> statement : statements) {
            execute(statement);
        }
    } catch(...) {
        
    }

    this->environment = pre;
}

void Interpreter::visitBlockStmt(shared_ptr<Block> stmt) {
    executeBlock(stmt->statements, environment);
}

void Interpreter::visitExpressionStmt(shared_ptr<Expression> stmt) {
    evaluate(stmt->expression);
    // we will temporarily print the output
    std::cout << stringify() << std::endl;
}

void Interpreter::visitAssignExpr(shared_ptr<Assign> expr) {
    evaluate(expr->value);
    environment->define(expr->name.lexeme, result);
}

void Interpreter::visitBinaryExpr(shared_ptr<Binary> expr) {
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

void Interpreter::interpret(std::vector<shared_ptr<Stmt>> statements) {
    try {
        for (shared_ptr<Stmt> statement : statements) {
            execute(statement);
        }
    } catch (RuntimeError& err) {
        Lua::runtimeError(err);
    }
}