#include <memory>
#include <string>
#include <variant>

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