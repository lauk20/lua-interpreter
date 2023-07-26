#include "Environment.hpp"
#include "RuntimeError.hpp"
#include <iostream>

void Environment::define(std::string name, variantX value) {
    values[name] = value;
}

variantX Environment::get(Token name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}