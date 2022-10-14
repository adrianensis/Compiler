#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Compiler/AST/AST.hpp"

class Compiler
{
public:
    Compiler() = default;
    void init();
    void addFile(const std::string& path);
    void compile();

private:
    AST mAST;
};

#endif