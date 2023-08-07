#pragma once

#include <memory>
#include <string>
#include <variant>
#include <unordered_map>

#include "Token.hpp"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "LuaCallable_Forward.hpp"

/// @brief Interpreter environment class
class Environment {
    //std::unordered_map<std::string, LiteralVal> values;

    /// @brief the enclosing (parent) environment
    std::shared_ptr<Environment> enclosing;

    public:
        /// @brief environment name, mostly used for debugging purposes
        std::string sname;

        /// @brief mapping of string to LiteralVal for variables
        std::unordered_map<std::string, LiteralVal> values;

        /// @brief Constructor using enclosing environment
        /// @param enclosing the parent environment
        Environment(std::shared_ptr<Environment> enclosing);

        /// @brief Constructor using enclosing environment and environment name
        /// @param enclosing the enclosing environment
        /// @param name the environment name
        Environment(std::shared_ptr<Environment> enclosing, std::string name);

        /// @brief Constructor using environment name
        /// @param name the environment name
        Environment(std::string name);
        
        /// @brief Default constructor
        Environment();

        /// @brief  Define a variable, searches up the environment stack if not found.
        ///         assigns to global environment if not found in enclosing environments
        /// @param name the variable name
        /// @param value the variable value
        void define(std::string name, LiteralVal value);

        /// @brief Define a local variable
        /// @param name name of the local variable
        /// @param value the value to assign it to
        void defineLocal(std::string name, LiteralVal value);

        /// @brief get value of the variable from the environment
        /// @param name Token of the identifier
        /// @return LiteralVal of the variable
        LiteralVal get(Token name);
};