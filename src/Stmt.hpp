#ifndef Stmt_H
#define Stmt_H

#include <vector>
#include <memory>
#include <variant>

#include "Expr.hpp"
#include "Token.hpp"

typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

class Stmt;
class StmtVisitor;
class Block;
class Expression;
class If;
class While;

class Stmt {

    public:
        virtual void accept(std::shared_ptr<StmtVisitor> visitor) = 0;

};

class StmtVisitor {
    public:
        virtual void visitBlockStmt(std::shared_ptr<Block> block) = 0;
        virtual void visitExpressionStmt(std::shared_ptr<Expression> expr) = 0;
        virtual void visitIfStmt(std::shared_ptr<If> ifstmt) = 0;
        virtual void visitWhileStmt(std::shared_ptr<While> whilestmt) = 0;
};

class Block : public Stmt, public std::enable_shared_from_this<Block> {
    public:
        std::vector<std::shared_ptr<Stmt>> statements;

        Block(std::vector<std::shared_ptr<Stmt>> statements);

        void accept(std::shared_ptr<StmtVisitor> visitor);
};

class Expression : public Stmt, public std::enable_shared_from_this<Expression> {
    public:
        std::shared_ptr<Expr> expression;

        Expression(std::shared_ptr<Expr> expression);

        void accept(std::shared_ptr<StmtVisitor> visitor);
};

class If : public Stmt, public std::enable_shared_from_this<If> {
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> thenBranch;
        std::vector<shared_ptr<Stmt>> elseifBranches;
        std::shared_ptr<Stmt> elseBranch;

        If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::vector<shared_ptr<Stmt>> elseifBranches, std::shared_ptr<Stmt> elseBranch);

        void accept(std::shared_ptr<StmtVisitor> visitor);
};

class While : public Stmt, public std::enable_shared_from_this<While> {
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> body;

        While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);

        void accept(std::shared_ptr<StmtVisitor> visitor);
};

/*
class Var : public Stmt, public std::enable_shared_from_this<Var> {
    public:
        Token name;
        std::shared_ptr<Expr> initializer;

        Var(Token name, std::shared_ptr<Expr> initializer);

        void accept(std::shared_ptr<StmtVisitor> visitor);
};
*/

#endif