#include <string>
#include <variant>
#include <unordered_map>

#include "Token.hpp"

typedef std::variant<double, std::string, bool, std::nullptr_t> variantX;

class Environment {
    std::unordered_map<std::string, variantX> values;

    public:
        void define(std::string name, variantX value);
        variantX get(Token name);
};