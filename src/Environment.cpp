#include "Environment.hpp"
#include "RuntimeError.hpp"
#include <iostream>

Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {

}

Environment::Environment() = default;

void Environment::define(std::string name, variantX value) {
    if (values.find(name) != values.end()) {
        values[name] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->define(name, value);
        return;
    }

    values[name] = value;
}

variantX Environment::get(Token name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}