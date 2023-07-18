#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace {
    /// @brief whitespace characters
    const char * ws = " \t\n\r";

    /// @brief trim trailing whitespace
    /// @param s the string to trim
    /// @param ws the whitespace characters
    /// @return the trimmed string
    std::string rtrim(std::string s, const char * t = ws){
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    /// @brief trim leading whitespace
    /// @param s the string to trim
    /// @param t the whitespace characters
    /// @return the trimmed string
    std::string ltrim(std::string s, const char * t = ws) {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    /// @brief split a string based on delimiter
    /// @param s the string to split
    /// @param delim the delimiter
    /// @return the split string
    std::string split(std::string& s, std::string&& delim) {
        return s.substr(0, s.find(delim));
    }

    void tokenize(std::string& s, char del, std::vector<std::string>& tokens) {
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof()) {
            std::getline(ss, word, del);
            tokens.push_back(rtrim(ltrim(word)));
        }
    }

    std::vector<std::string> tokenize(std::string& s, char del) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof()) {
            std::getline(ss, word, del);
            tokens.push_back(rtrim(ltrim(word)));
        }

        return tokens;
    }

    void write(std::ofstream& os, std::string text) {
        os << text << std::endl;
    }

    /*
    void write(std::ofstream& os, char * text) {
        os << text << std::endl;
    }
    */

    void writeln(std::ofstream& os) {
        os << std::endl;
    }

    void defineVisitor(std::ofstream& file, std::string& basename, std::vector<std::string> types) {
        write(file, "template <class T>");
        write(file, "class " + basename + "Visitor {");
        write(file, "    public:");

        std::string lowerBasename;
        for (auto& c : basename) {
            lowerBasename += tolower(c);
        }
        for (std::string type : types) {
            std::string typeName = rtrim(split(type, ":"));
            write(file,"        virtual T visit" + typeName + basename + "(shared_ptr<" + typeName + "<T>> " + lowerBasename + ") = 0;");
        }

        write(file, "};");
    }

    void defineType(std::ofstream& file, std::string& basename, std::string& classname, std::string fields) {
        // Class definition
        write(file, "template <class T>");
        write(file, "class " + classname + " : public " + basename + "<T> {");
        
        std::vector<std::string> fieldTokens;
        tokenize(fields, ',', fieldTokens);

        write(file, "    public:");
        // Instance variables
        for (std::string field : fieldTokens) {
            write(file, "        " + field + ";");
        }

        writeln(file);

        // Constructor
        write(file, "        " + classname + "(" + fields + ") {");
        
        // Set instance variables
        for (std::string& field : fieldTokens) {
            std::string name = tokenize(field, ' ')[1];
            write(file, "            this->" + name + " = " + name + ";");
        }

        write(file, "        }" );

        // Implement accept
        writeln(file);
        write(file, "        T accept(shared_ptr<" + basename + "Visitor<T>> visitor) {");
        write(file, "            return visitor->visit" + classname + basename + "(this->shared_from_this());");
        write(file, "        }");

        write(file, "};");
    }

    void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types) {
        std::string path = outputDir + "/" + baseName + ".cpp";
        std::ofstream file;
        file.open(path);

        // Includes
        write(file, "#include <any>");
        write(file, "#include <vector>");
        write(file, "#include <memory>");
        writeln(file);
        write(file, "#include \"Token.hpp\"");
        writeln(file);
        write(file, "using std::shared_ptr;");
        writeln(file);

        // Expr class
        write(file, "template <class T>");
        write(file, "class " + baseName + " {");
        writeln(file);
        write(file, "    public:");
        write(file, "        virtual T accept(Visitor<T> visitor) = 0;");
        writeln(file);
        write(file, "};");
        writeln(file);

        // Visitor
        defineVisitor(file, baseName, types); 
        writeln(file);

        // AcceptClass
        /*
        write(file, "class " + baseName + "Accept {");
        write(file, "       virtual void accept(Visitor<std::any> visitor) = 0;");
        write(file, "};");
        writeln(file);
        */

        for (std::string type : types) {
            std::string classname = rtrim(split(type, ":"));
            std::string fields = ltrim(type.substr(type.find(":") + 1));
            defineType(file, baseName, classname, fields);
            writeln(file);
        }

        file.close();
    }
}

/// @brief generate abstract syntax tree .cpp file
/// @param argc num cmdline args
/// @param argv cmdline args
/// @return 0 if success, any other number otherwise
int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Usage: generate_ast <output directory>" << std::endl;
        return 1;
    }
    std::string outputDir = argv[1];
    defineAst(outputDir, "Expr", {
        "Binary   : shared_ptr<Expr<T>> left, Token op, shared_ptr<Expr<T>> right",
        "Grouping : shared_ptr<Expr<T>> expression",
        "Literal  : T value",
        "Unary    : Token op, shared_ptr<Expr<T>> right"
    });

    return 0;
}