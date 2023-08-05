#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <string>

#include "Literal.hpp"
#include "Interpreter_Forward.hpp"
class LuaCallable;

class LuaCallable : public std::enable_shared_from_this<LuaCallable> {
    public:
        virtual int arity() = 0;
        virtual LiteralVal call(std::shared_ptr<Interpreter> interpreter, std::vector<LiteralVal> arguments) = 0;
};