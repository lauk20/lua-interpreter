#include <string>

#include "Expr.hpp"

using std::shared_ptr;
using std::string;

class AstPrinter : public ExprVisitor, public std::enable_shared_from_this<AstPrinter> {
    string result;

    public:
        void print(shared_ptr<Expr> expr);

        void visitBinaryExpr(shared_ptr<Binary> expr);

        void visitGroupingExpr(shared_ptr<Grouping> expr);

        void visitLiteralExpr(shared_ptr<Literal> expr);

        void visitUnaryExpr(shared_ptr<Unary> expr);
};