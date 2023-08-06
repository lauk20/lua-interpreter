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

/// @brief Interpreter class for interpreting expressions and statements
class Interpreter : public ExprVisitor, public StmtVisitor, public std::enable_shared_from_this<Interpreter> {
    /// @brief result holds the LiteralVal of the previously evaluated expression.
    LiteralVal result;

    /// @brief Determine whether a LiteralVal is true or not
    /// @param object LiteralVal to identify truthiness
    /// @return truthiness of the LiteralVal
    bool isTruthy(LiteralVal object);

    /// @brief Determine whether a LiteralVal is equal to another LiteralVal
    /// @param left Object to compare
    /// @param right Object to compare
    /// @return true if equal, false otherwise.
    bool isEqual(LiteralVal left, LiteralVal right);

    public:
        /// @brief get string representation of the result
        /// @return string representation of the result
        std::string stringify();

        /// @brief current interpreter Environment
        std::shared_ptr<Environment> environment;

        /// @brief the global interpreter Environment
        std::shared_ptr<Environment> globalEnvironment;

        /// @brief Construct Interpreter object with given environment
        /// @param environment environment to interpret in
        Interpreter(shared_ptr<Environment> environment);

        /// @brief visit Literal expr
        /// @param expr Literal to visit
        void visitLiteralExpr(shared_ptr<Literal> expr);

        /// @brief visit Logical expr
        /// @param expr Logical expr to visit
        void visitLogicalExpr(shared_ptr<Logical> expr);

        /// @brief visit Unary expr
        /// @param expr Unary expr to visit
        void visitUnaryExpr(shared_ptr<Unary> expr);

        /// @brief visit Variable expr
        /// @param expr Variable expr to visit
        void visitVariableExpr(shared_ptr<Variable> expr);

        /// @brief determine whether the LiteralVal holds a number. throws error if not.
        /// @param op operator
        /// @param operand the LiteralVal to check
        void checkNumberOperand(Token op, LiteralVal operand);

        /// @brief determine whether the two LiteralVal hold a number. throws error if not.
        /// @param op operator.
        /// @param left the first LiteralVal
        /// @param right the second LiteralVal
        void checkNumberOperands(Token op, LiteralVal left, LiteralVal right);

        /// @brief visit Grouping expr
        /// @param expr Grouping expr to visit
        void visitGroupingExpr(shared_ptr<Grouping> expr);

        /// @brief evaluate an expression
        /// @param expr the expression to evaluate
        void evaluate(shared_ptr<Expr> expr);

        /// @brief execute a statement
        /// @param stmt the statement to execute
        void execute(shared_ptr<Stmt> stmt);

        /// @brief execute multiple statements (a block)
        /// @param statements vector of statements to execute
        /// @param environment the environment to execute in
        void executeBlock(std::vector<shared_ptr<Stmt>> statements, shared_ptr<Environment> environment);

        /// @brief visit Block statement
        /// @param stmt the Block statement
        void visitBlockStmt(shared_ptr<Block> stmt);

        /// @brief visit Expression statement
        /// @param stmt the Expressions statement
        void visitExpressionStmt(shared_ptr<Expression> stmt);

        /// @brief visit Function statement
        /// @param stmt the Function statement
        void visitFunctionStmt(shared_ptr<Function> stmt);

        /// @brief visit While statement
        /// @param stmt the While statement
        void visitWhileStmt(shared_ptr<While> stmt);

        /// @brief visit Var statement
        /// @param stmt the Var statement
        void visitVarStmt(shared_ptr<Var> stmt);

        /// @brief visit If statement
        /// @param stmt the If statement
        void visitIfStmt(shared_ptr<If> stmt);
        
        /// @brief visit Return statement
        /// @param stmt the Return statement
        void visitReturnStmt(shared_ptr<Return> stmt);

        /// @brief visit Assign expr
        /// @param expr the Assign expr
        void visitAssignExpr(shared_ptr<Assign> expr);

        /// @brief visit Binary expr
        /// @param expr the Binary expr
        void visitBinaryExpr(shared_ptr<Binary> expr);

        /// @brief visit Call expr
        /// @param expr the Call expr
        void visitCallExpr(shared_ptr<Call> expr);

        /// @brief run the interpreter
        /// @param expression statements to interpret
        void interpret(std::vector<shared_ptr<Stmt>> expression);
};