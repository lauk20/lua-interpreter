#ifndef RuntimeError_H
#define RuntimeError_H

#include <exception>
#include <string>

#include "Token.hpp"

class RuntimeError : public std::exception {
    public:
        Token token;
        std::string message;

        RuntimeError(Token token, std::string message);
        const char * what();
};

#endif