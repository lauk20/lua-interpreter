#ifndef Environment_H
#define Environment_H

#include <memory>
#include <string>
#include <variant>
#include <unordered_map>

#include "Token.hpp"

typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

class Environment {
    std::unordered_map<std::string, variantX> values;
    std::shared_ptr<Environment> enclosing;

    public:
        Environment(std::shared_ptr<Environment> enclosing);
        Environment();
        void define(std::string name, variantX value);
        variantX get(Token name);
};

#endif