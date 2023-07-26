#ifndef ParseError_H
#define ParseError_H

#include <exception>
#include <memory>
#include <vector>

using std::shared_ptr;

class ParseError : std::exception {
    public:
        ParseError();
};

#endif