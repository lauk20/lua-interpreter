#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Lua.hpp"
#include "Scanner.cpp"

bool hadError = false;

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
        Scanner* scanner = new Scanner(source);
        std::vector<Token> tokens = scanner->scanTokens();

        for (Token token : tokens) {
            std::cout << token;
        }
    }

    void error(int line, std::string message) {
        report(line, "", message);
    }

    void report(int line, std::string where, std::string message) {
        std::cout << "[line " << line << "]" << "Error" << where << ": " << message << std::endl;
        hadError = true;
    }
}

int main(int argc, char * argv[]) {
    if (argc == 2) {
        std::cout << "Usage: lua [script]";
    } else if (argc == 2) {
        Lua::runFile(argv[1]);
    } else {
        Lua::runPrompt();
    }
}