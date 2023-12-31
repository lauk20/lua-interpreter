#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Environment.hpp"
#include "Interpreter.hpp"
#include "Lua.hpp"
#include "Parser.hpp"
#include "RuntimeError.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

using std::make_shared;

bool hadError = false;
bool hadRuntimeError = false;
shared_ptr<Environment> globalEnv = make_shared<Environment>("global");
shared_ptr<Interpreter> interpreter = make_shared<Interpreter>(globalEnv);

namespace Lua {
    void runFile(std::string path) {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
    }

    void runPrompt() {
        std::string input;

        while (1) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.size() == 0) break;
            run(input);
        }
    }

    void run(std::string source) {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scanTokens();
        Parser parser(tokens);
        std::vector<shared_ptr<Stmt>> statements = parser.parse();

        if (hadError) return;

        interpreter->interpret(statements);

        if (hadRuntimeError) return;

        //make_shared<AstPrinter>()->print(expression);
    }

    void error(int line, std::string message) {
        report(line, "", message);
    }

    void runtimeError(RuntimeError& error) {
        std::cout << error.what() << std::endl;
    }

    void report(int line, std::string where, std::string message) {
        std::cout << "[line " << line << "] " << "Error" << where << ": " << message << std::endl;
        hadError = true;
    }

    void error(Token token, std::string message) {
        if (token.type == EOFILE) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc > 2) {
        std::cout << "Usage: lua [script]";
    } else if (argc == 2) {
        Lua::runFile(argv[1]);
    } else {
        Lua::runPrompt();
    }
}