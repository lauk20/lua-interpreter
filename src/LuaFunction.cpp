#include "LuaFunction.hpp"
#include "Environment.hpp"
#include "Return.hpp"

LuaFunction::LuaFunction(shared_ptr<Function> declaration) : declaration(declaration) {
    
}

int LuaFunction::arity() {
    return declaration->params.size();
}

LiteralVal LuaFunction::call(shared_ptr<Interpreter> interpreter, std::vector<LiteralVal> arguments) {
    //std::cout << "create new env" << declaration->name.lexeme<< std::endl;
    shared_ptr<Environment> environment = std::make_shared<Environment>(interpreter->globalEnvironment, declaration->name.lexeme);
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->defineLocal(declaration->params[i].lexeme, LiteralVal(arguments[i]));
    }

    try {
        interpreter->executeBlock({declaration->body}, environment);
    } catch(ReturnException returnValue) {
        return returnValue.value;
    }

    return LiteralVal(nullptr);
}