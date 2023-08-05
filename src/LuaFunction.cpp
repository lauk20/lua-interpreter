#include "LuaFunction.hpp"
#include "Environment.hpp"

LuaFunction::LuaFunction(shared_ptr<Function> declaration) : declaration(declaration) {
    
}

int LuaFunction::arity() {
    return declaration->params.size();
}

LiteralVal LuaFunction::call(shared_ptr<Interpreter> interpreter, std::vector<LiteralVal> arguments) {
    shared_ptr<Environment> environment = std::make_shared<Environment>(interpreter->globalEnvironment);
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    interpreter->executeBlock({declaration->body}, environment);

    return LiteralVal(nullptr);
}