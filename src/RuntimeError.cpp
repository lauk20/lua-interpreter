#include "RuntimeError.hpp"

RuntimeError::RuntimeError(Token token, std::string message) : token(token), message(message) {
}

const char * RuntimeError::what() {
    return message.c_str();
}