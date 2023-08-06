#include <memory>
#include <string>
#include <variant>
#include <iostream>

#include "Literal.hpp"

LiteralVal::LiteralVal(double value) : value(value) {

}

LiteralVal::LiteralVal(std::string value) : value(value) {

}

LiteralVal::LiteralVal(bool value) : value(value) {

}

LiteralVal::LiteralVal(std::nullptr_t value) : value(value) {

}

LiteralVal::LiteralVal(std::variant<double, std::string, bool, std::nullptr_t, std::shared_ptr<LuaCallable>> value) : value(value) {
    
}

LiteralVal::LiteralVal() : value(nullptr) {

}

std::ostream& operator<<(std::ostream& os, const LiteralVal& val) {
    if (std::holds_alternative<double>(val.value)) {
        os << std::to_string(std::get<double>(val.value));
        return os;
    }

    if (std::holds_alternative<std::string>(val.value)) {
        os << std::get<std::string>(val.value);
        return os;
    }

    if (std::holds_alternative<bool>(val.value)) {
        if (std::get<bool>(val.value)) {
            os << "true";
            return os;
        }
        os << "false";
        return os;
    }

    if (std::holds_alternative<nullptr_t>(val.value)) {
        os << "nil";
        return os;
    }

    if (std::holds_alternative<std::shared_ptr<LuaCallable>>(val.value)) {
        os << "func";
        return os;
    } 

    os << "undefined";
    return os;
}