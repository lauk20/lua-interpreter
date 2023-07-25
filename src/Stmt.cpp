#include "Stmt.hpp"

Expression::Expression(std::shared_ptr<Expr> expression) : expression(expression) {
    
}

void Expression::accept(std::shared_ptr<StmtVisitor> visitor) {
    visitor->visitExpressionStmt(shared_from_this());
}