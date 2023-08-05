#pragma once

#include <memory>
#include <string>
#include <variant>

#include "LuaCallable_Forward.hpp"
#include "LuaFunction_Forward.hpp"

class LiteralVal {
    public:
        std::variant<double, std::string, bool, std::nullptr_t, std::shared_ptr<LuaCallable>> value;

        LiteralVal(double);
        LiteralVal(std::string);
        LiteralVal(bool);
        LiteralVal(std::nullptr_t);
        LiteralVal(std::variant<double, std::string, bool, std::nullptr_t, std::shared_ptr<LuaCallable>>);
        LiteralVal();
};