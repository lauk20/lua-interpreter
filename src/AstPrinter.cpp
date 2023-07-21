#include <string>

#include "Expr.cpp"

using std::shared_ptr;
using std::string;

class AstPrinter : public ExprVisitor<string>, public std::enable_shared_from_this<AstPrinter> {
    public:
        string print(shared_ptr<Expr<string>> expr) {
            return expr->accept(this->shared_from_this());
        }

        string visitBinaryExpr(shared_ptr<Binary<string>> expr) {
            return expr->op.lexeme + " " + expr->left->accept(shared_from_this()) + " " + expr->right->accept(shared_from_this());
        }

        string visitGroupingExpr(shared_ptr<Grouping<string>> expr) {
            return "group " + expr->expression->accept(shared_from_this());
        }

        string visitLiteralExpr(shared_ptr<Literal<string>> expr) {
            if (expr->value.index() == 0) {
                return std::to_string(std::get<0>(expr->value));
            }

            return std::get<1>(expr->value);
        }

        string visitUnaryExpr(shared_ptr<Unary<string>> expr) {
            return expr->op.lexeme + " " + expr->right->accept(shared_from_this());
        }
};

using std::make_shared;
int main(void) {
    shared_ptr<Expr<string>> expression = make_shared<Binary<string>>(
        make_shared<Unary<string>>(
            Token(MINUS, "-", NULL, 1),
            make_shared<Literal<string>>(123)
        ),
        Token(STAR, "*", NULL, 1),
        make_shared<Grouping<string>>(
            make_shared<Literal<string>>(45.67)
        )
    );

    std::cout << "HERE" << std::endl;

    std::cout << make_shared<AstPrinter>()->print(expression);
}