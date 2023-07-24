#include <memory>
#include <string>
#include <stdexcept>

#include "Expr.hpp"
#include "Lua.hpp"
#include "RuntimeError.hpp"

using std::shared_ptr;
typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

class Interpreter : public ExprVisitor, public std::enable_shared_from_this<Interpreter> {
    variantX result;

    std::string stringify();

    bool isTruthy(variantX object);

    bool isEqual(variantX left, variantX right);

    public:
        void visitLiteralExpr(shared_ptr<Literal> expr);

        void visitUnaryExpr(shared_ptr<Unary> expr);

        void checkNumberOperand(Token op, variantX operand);

        void checkNumberOperands(Token op, variantX left, variantX right);

        void visitGroupingExpr(shared_ptr<Grouping> expr);

        void evaluate(shared_ptr<Expr> expr);

        void visitBinaryExpr(shared_ptr<Binary> expr);

        void interpret(shared_ptr<Expr> expression);
};