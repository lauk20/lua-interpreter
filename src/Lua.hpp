#include <string>

namespace Lua {
    /// run Lua code from source code file after reading the file
    /// @param path The string path of the source code file
    ///
    void runFile(std::string path);

    /// run Lua code from the REPL
    ///
    void runPrompt();

    /// run the source code from the file or REPL
    /// @param source 
    ///
    void run(std::string source);

    /// report errors to print to stdout
    /// @param line line where the error occurs
    /// @param message message of the error
    ///
    void error(int line, std::string message) {
        report(line, "", message);
    }

    /// print out error message to stdout
    /// @param line the line where the error occurs
    /// @param where type of error
    /// @param message error message
    void report(int line, std::string where, std::string message);
}