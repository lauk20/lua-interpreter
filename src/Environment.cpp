#include "Environment.hpp"
#include "RuntimeError.hpp"
#include "Literal.hpp"
#include <iostream>

Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {

}

Environment::Environment(std::shared_ptr<Environment> enclosing, std::string name) : enclosing(enclosing), sname(name) {

}

Environment::Environment(std::string name) : sname(name) {

}

Environment::Environment() = default;

void Environment::define(std::string name, LiteralVal value) {
    //std::cout << "in env: " << sname << std::endl;
    if (values.find(name) != values.end()) {
        //std:: cout << "defineGlobal <locally>: " << name << " : " << value << std::endl;
        values[name] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->define(name, value);
        return;
    }

    values[name] = value;
}

void Environment::defineLocal(std::string name, LiteralVal value) {
    //std::cout << "in DL env: " << sname << std::endl;
    //std::cout << "defineLocal : " << name << " : " << value << std::endl;
    values[name] = value;
}

LiteralVal Environment::get(Token name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}