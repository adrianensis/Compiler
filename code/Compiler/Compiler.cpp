#include "Compiler/Compiler.hpp"

void Compiler::init()
{
}

void Compiler::addFile(const std::string& path)
{
    mAST.addFile(path);
}

void Compiler::compile()
{
    mAST.generateCode();
}