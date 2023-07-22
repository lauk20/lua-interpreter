#include <exception>
#include <memory>
#include <vector>

using std::shared_ptr;

class ParseError : std::exception {
    public:
        ParseError();
};