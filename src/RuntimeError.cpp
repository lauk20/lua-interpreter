#include "RuntimeError.hpp"

RuntimeError::RuntimeError(Token token, std::string_view message) : token(token), message(message) {
    
}

const char * RuntimeError::what() {
    return message.data();
}