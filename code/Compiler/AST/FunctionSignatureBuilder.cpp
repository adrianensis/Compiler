#include "Compiler/AST/FunctionSignatureBuilder.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"

void FunctionSignatureBuilder::push(const std::string& string)
{
    mSignatureVector.push_back(string);
    regenerateSignature();
}

void FunctionSignatureBuilder::regenerateSignature()
{
    mSignature.clear();
    for (const std::string &signature: mSignatureVector)
    {
        if(!mSignature.empty())
        {
            mSignature += ";";
        }
        mSignature += signature;
    }
}
