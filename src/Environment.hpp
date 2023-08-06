#ifndef Environment_H
#define Environment_H

#include <memory>
#include <string>
#include <variant>
#include <unordered_map>

#include "Token.hpp"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "LuaCallable_Forward.hpp"

class Environment {
    //std::unordered_map<std::string, LiteralVal> values;
    std::shared_ptr<Environment> enclosing;

    public:
        std::string sname;
        std::unordered_map<std::string, LiteralVal> values;
        Environment(std::shared_ptr<Environment> enclosing);
        Environment(std::shared_ptr<Environment> enclosing, std::string name);
        Environment(std::string name);
        Environment();
        void define(std::string name, LiteralVal value);
        void defineLocal(std::string name, LiteralVal value);
        LiteralVal get(Token name);

};
#endif