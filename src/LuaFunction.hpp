#ifndef LuaFunction_H
#define LuaFunction_H

#include <vector>
#include <memory>
#include "Interpreter.hpp"
#include "Stmt.hpp"
#include "LuaCallable.hpp"

class LuaFunction : public LuaCallable , public std::enable_shared_from_this<LuaFunction> {
    public:
        shared_ptr<Function> declaration;

        LuaFunction(shared_ptr<Function> declaration);

        int arity();

        LiteralVal call(shared_ptr<Interpreter> interpreter, std::vector<LiteralVal> arguments);
};

#endif