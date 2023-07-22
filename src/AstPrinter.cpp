#include <string>

#include "Expr.cpp"

using std::shared_ptr;
using std::string;

class AstPrinter : public ExprVisitor, public std::enable_shared_from_this<AstPrinter> {
    string result;

    public:
        void print(shared_ptr<Expr> expr) {
            expr->accept(this->shared_from_this());
            std::cout << result << std::endl;
        }

        void visitBinaryExpr(shared_ptr<Binary> expr) {
            result += "(" + expr->op.lexeme + " ";
            expr->left->accept(shared_from_this());
            result += " ";
            expr->right->accept(shared_from_this());
            result += ")";
        }

        void visitGroupingExpr(shared_ptr<Grouping> expr) {
            result += "(group "; 
            expr->expression->accept(shared_from_this());
            result += ")";
        }

        void visitLiteralExpr(shared_ptr<Literal> expr) {
            if (expr->value.index() == 0) {
                result += std::to_string(std::get<0>(expr->value));
                return;
            }

            result += std::get<1>(expr->value);
        }

        void visitUnaryExpr(shared_ptr<Unary> expr) {
            result += "(" + expr->op.lexeme + " "; 
            expr->right->accept(shared_from_this());
            result += ")";
        }
};

using std::make_shared;
int main(void) {
    shared_ptr<Expr> expression = make_shared<Binary>(
        make_shared<Unary>(
            Token(MINUS, "-", NULL, 1),
            make_shared<Literal>(123)
        ),
        Token(STAR, "*", NULL, 1),
        make_shared<Grouping>(
            make_shared<Literal>(45.67)
        )
    );

    make_shared<AstPrinter>()->print(expression);

    return 0;
}