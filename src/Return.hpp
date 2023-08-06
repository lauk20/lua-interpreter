#pragma once

#include <exception>
#include <string>
#include "Literal.hpp"

class ReturnException : public std::exception {
    public:
        LiteralVal value;

        ReturnException(LiteralVal value);
};