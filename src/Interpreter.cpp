#include <memory>
#include <string>
#include <stdexcept>

#include "Interpreter.hpp"
#include "Literal.hpp"
#include "LuaCallable.hpp"
#include "Environment.hpp"

using std::shared_ptr;

class PrintFunction : public LuaCallable, std::enable_shared_from_this<PrintFunction> {
    public:
        PrintFunction() = default;

        int arity() {
            return 1;
        }

        LiteralVal call(shared_ptr<Interpreter> interpreter, std::vector<LiteralVal> arguments) {
            std::cout << interpreter->stringify() << std::endl;
            return LiteralVal(nullptr);
        }
};

Interpreter::Interpreter(shared_ptr<Environment> environment) : environment(environment), globalEnvironment(environment) {   
    environment->define("print", LiteralVal(std::make_shared<PrintFunction>()));
}

std::string Interpreter::stringify() {
    if (std::holds_alternative<double>(result.value)) {
        return std::to_string(std::get<double>(result.value));
    }

    if (std::holds_alternative<std::string>(result.value)) {
        return std::get<std::string>(result.value);
    }

    if (std::holds_alternative<bool>(result.value)) {
        if (std::get<bool>(result.value)) {
            return "true";
        }
        return "false";
    }

    if (std::holds_alternative<nullptr_t>(result.value)) {
        return "nil";
    }

    return "undefined";
}

bool Interpreter::isTruthy(LiteralVal object) {
    if (object.value.index() == 3) return false;
    if (object.value.index() == 2) return std::get<2>(object.value);
    return true;
}

bool Interpreter::isEqual(LiteralVal left, LiteralVal right) {
    if (left.value.index() == 3 && right.value.index() == 3) return true;
    if (left.value.index() == 3 || right.value.index() == 3) return false;
    return left.value == right.value;
}

void Interpreter::visitLiteralExpr(shared_ptr<Literal> expr) {
    result = expr->value;
}

void Interpreter::visitLogicalExpr(shared_ptr<Logical> expr) {
    evaluate(expr->left);

    if (expr->op.type == OR) {
        if (isTruthy(result)) {
            return;
        }
    } else {
        if (!isTruthy(result)) {
            return;
        }
    }

    evaluate(expr->right);
}

void Interpreter::visitUnaryExpr(shared_ptr<Unary> expr) {
    evaluate(expr->right); // this will set result.value to the right operand

    switch (expr->op.type) {
        case MINUS:
            checkNumberOperand(expr->op, result);
            result.value = -(std::get<0>(result.value));
            break;
        case TILDE:
            result.value = !(isTruthy(result));
    }
}

void Interpreter::visitVariableExpr(shared_ptr<Variable> expr) {
    result = environment->get(expr->name);
}

void Interpreter::checkNumberOperand(Token op, LiteralVal operand) {
    if (std::holds_alternative<double>(operand.value)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, LiteralVal left, LiteralVal right) {
    if (std::holds_alternative<double>(left.value) && std::holds_alternative<double>(right.value)) {
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
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
}

void Interpreter::visitExpressionStmt(shared_ptr<Expression> stmt) {
    evaluate(stmt->expression);
    // we will temporarily print the output
    //std::cout << stringify() << std::endl;
}

void Interpreter::visitFunctionStmt(shared_ptr<Function> stmt) {
    shared_ptr<LuaFunction> function = std::make_shared<LuaFunction>(stmt);
    environment->define(stmt->name.lexeme, LiteralVal(function));
}

void Interpreter::visitWhileStmt(shared_ptr<While> stmt) {
    evaluate(stmt->condition);
    while (isTruthy(result)) {
        execute(stmt->body);
        evaluate(stmt->condition);
    }
}

void Interpreter::visitVarStmt(shared_ptr<Var> stmt) {
    LiteralVal value(nullptr);
    if (stmt->initializer != nullptr) {
        evaluate(stmt->initializer);
        value = result;
    }

    environment->defineLocal(stmt->name.lexeme, value);
}

void Interpreter::visitIfStmt(shared_ptr<If> stmt) {
    evaluate(stmt->condition);
    if (isTruthy(result)) {
        execute(stmt->thenBranch);
    } else if (stmt->elseifBranches.size() != 0) {
        for (auto e : stmt->elseifBranches) {
            shared_ptr<If> element = std::static_pointer_cast<If>(e);
            evaluate(element->condition);
            if (isTruthy(result)) {
                execute(element->thenBranch);
                break;
            }
        }
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
}

void Interpreter::visitAssignExpr(shared_ptr<Assign> expr) {
    evaluate(expr->value);
    environment->define(expr->name.lexeme, result);
}

void Interpreter::visitBinaryExpr(shared_ptr<Binary> expr) {
    evaluate(expr->left);
    LiteralVal left = result;
    evaluate(expr->right);
    LiteralVal right = result;

    switch (expr->op.type) {
        case MINUS:
            checkNumberOperands(expr->op, left, right);
            result.value = (double) (std::get<double>(left.value) - std::get<double>(right.value));
            break;
        case SLASH:
            checkNumberOperands(expr->op, left, right);
            result.value = (double) (std::get<double>(left.value) / std::get<double>(right.value));
            break;
        case STAR:
            checkNumberOperands(expr->op, left, right);
            result.value = (double) (std::get<double>(left.value) * std::get<double>(right.value));
            break;
        case PLUS:
            checkNumberOperands(expr->op, left, right);
            result.value = (double) (std::get<double>(left.value) + std::get<double>(right.value));
            break;
        case GREATER:
            checkNumberOperands(expr->op, left, right);
            result.value = (bool) (std::get<double>(left.value) > std::get<double>(right.value));
            break;
        case GREATER_EQUAL:
            checkNumberOperands(expr->op, left, right);
            result.value = (bool) (std::get<double>(left.value) >= std::get<double>(right.value));
            break;
        case LESS:
            checkNumberOperands(expr->op, left, right);
            result.value = (bool) (std::get<double>(left.value) < std::get<double>(right.value));
            break;
        case LESS_EQUAL:
            checkNumberOperands(expr->op, left, right);
            result.value = (bool) (std::get<double>(left.value) <= std::get<double>(right.value));
            break;
        case TILDE_EQUAL:
            result.value = (bool) (!(isEqual(left, right)));
            break;
        case EQUAL_EQUAL:
            result.value = (bool) (isEqual(left, right));
            break;
    }
}

void Interpreter::visitCallExpr(shared_ptr<Call> expr) {
    evaluate(expr->callee);
    LiteralVal callee = result;

    std::vector<LiteralVal> arguments;
    for (shared_ptr<Expr> argument : expr->arguments) {
        evaluate(argument);
        arguments.push_back(result);
    }

    if (!std::holds_alternative<shared_ptr<LuaCallable>>(callee.value)) {
        throw RuntimeError(expr->paren, "Can only call functions.");
    }

    shared_ptr<LuaCallable> function = std::get<shared_ptr<LuaCallable>>(callee.value);
    if (arguments.size() != function->arity()) {
        throw RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }

    function->call(shared_from_this(), arguments);
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