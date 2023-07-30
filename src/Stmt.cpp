#include "Stmt.hpp"

Block::Block(std::vector<std::shared_ptr<Stmt>> statements) : statements(statements) {

}

void Block::accept(std::shared_ptr<StmtVisitor> visitor) {
    visitor->visitBlockStmt(shared_from_this());
}

Expression::Expression(std::shared_ptr<Expr> expression) : expression(expression) {
    
}

void Expression::accept(std::shared_ptr<StmtVisitor> visitor) {
    visitor->visitExpressionStmt(shared_from_this());
}

If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::vector<shared_ptr<Stmt>> elseifBranches, std::shared_ptr<Stmt> elseBranch)
    : condition(condition), thenBranch(thenBranch), elseifBranches(elseifBranches), elseBranch(elseBranch) {
    
}

void If::accept(std::shared_ptr<StmtVisitor> visitor) {
    visitor->visitIfStmt(shared_from_this());
}

While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) : condition(condition), body(body) {

}

void While::accept(std::shared_ptr<StmtVisitor> visitor) {
    visitor->visitWhileStmt(shared_from_this());
}