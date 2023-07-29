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