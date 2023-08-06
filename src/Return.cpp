#include "Return.hpp"

ReturnException::ReturnException(LiteralVal value) : value(value) {
    this->value = value;
}