#pragma once

#include <memory>
#include <string>
#include <stdexcept>

#include "Environment_Forward.hpp"
#include "Expr.hpp"
#include "Literal.hpp"
#include "Lua.hpp"
#include "LuaCallable.hpp"
#include "LuaFunction.hpp"
#include "RuntimeError.hpp"
#include "Stmt.hpp"

using std::shared_ptr;

class Interpreter : public ExprVisitor, public StmtVisitor, public std::enable_shared_from_this<Interpreter> {
    LiteralVal result;

    bool isTruthy(LiteralVal object);

    bool isEqual(LiteralVal left, LiteralVal right);

    public:
        std::string stringify();

        std::shared_ptr<Environment> environment;

        std::shared_ptr<Environment> globalEnvironment;

        Interpreter(shared_ptr<Environment> environment);

        void visitLiteralExpr(shared_ptr<Literal> expr);

        void visitLogicalExpr(shared_ptr<Logical> expr);

        void visitUnaryExpr(shared_ptr<Unary> expr);

        void visitVariableExpr(shared_ptr<Variable> expr);

        void checkNumberOperand(Token op, LiteralVal operand);

        void checkNumberOperands(Token op, LiteralVal left, LiteralVal right);

        void visitGroupingExpr(shared_ptr<Grouping> expr);

        void evaluate(shared_ptr<Expr> expr);

        void execute(shared_ptr<Stmt> stmt);

        void executeBlock(std::vector<shared_ptr<Stmt>> statements, shared_ptr<Environment> environment);

        void visitBlockStmt(shared_ptr<Block> stmt);

        void visitExpressionStmt(shared_ptr<Expression> stmt);

        void visitFunctionStmt(shared_ptr<Function> stmt);

        void visitWhileStmt(shared_ptr<While> stmt);

        void visitVarStmt(shared_ptr<Var> stmt);

        void visitIfStmt(shared_ptr<If> stmt);

        void visitAssignExpr(shared_ptr<Assign> expr);

        void visitBinaryExpr(shared_ptr<Binary> expr);

        void visitCallExpr(shared_ptr<Call> expr);

        void interpret(std::vector<shared_ptr<Stmt>> expression);
};